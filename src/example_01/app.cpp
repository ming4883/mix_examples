#include <mix/mix_application.h>
#include <mix/mix_frontend.h>
#include <mix/mix_asset.h>
#include <bgfx.h>
#include <math.h>

namespace example
{
    class TheApplication : public mix::Application
    {
    public:
        struct Triangle
        {
            static bgfx::VertexDecl sharedVtxDecl;

            bgfx::DynamicVertexBufferHandle vtxbuf;
            bgfx::DynamicIndexBufferHandle idxbuf;
            uint16_t idxmem[3];
            float vtxmem[5*3];

            Triangle()
                : vtxbuf (BGFX_INVALID_HANDLE)
                , idxbuf (BGFX_INVALID_HANDLE)
            {
            }

            void init()
            {
                if (0 == sharedVtxDecl.getStride())
                {
                    sharedVtxDecl.begin()
                        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
                        .end();
                }

                vtxbuf = bgfx::createDynamicVertexBuffer (3, sharedVtxDecl, BGFX_BUFFER_NONE);
                idxbuf = bgfx::createDynamicIndexBuffer (3, BGFX_BUFFER_NONE);
            }

            void update (float _t)
            {
                float _s = cosf (_t) * 0.5f + 0.5f;
                _s = 0.25f + 0.75f * _s;

                idxmem[0] = 0u;
                idxmem[1] = 1u;
                idxmem[2] = 2u;

                vtxmem[ 0] = 0.0f; vtxmem[ 1] = _s; vtxmem[ 2] = 0.5f; vtxmem[ 3] = 0.5f; vtxmem[ 4] = 1.0f;
                vtxmem[ 5] =-_s; vtxmem[ 6] =-_s; vtxmem[ 7] = 0.5f; vtxmem[ 8] = 0.0f; vtxmem[ 9] = 0.0f;
                vtxmem[10] = _s; vtxmem[11] =-_s; vtxmem[12] = 0.5f; vtxmem[13] = 1.0f; vtxmem[14] = 0.0f;

                bgfx::updateDynamicVertexBuffer (vtxbuf, 0, bgfx::makeRef (vtxmem, sizeof (vtxmem)));
                bgfx::updateDynamicIndexBuffer  (idxbuf, 0, bgfx::makeRef (idxmem, sizeof (idxmem)));
            }

            void shutdown()
            {
                bgfx::destroyDynamicVertexBuffer (vtxbuf);
                bgfx::destroyDynamicIndexBuffer (idxbuf);
            }
        };

        struct CheckerTexture
        {
            bgfx::TextureHandle tex;

            CheckerTexture()
            : tex (BGFX_INVALID_HANDLE)
            {

            }

            void init()
            {
                tex = bgfx::createTexture2D (16u, 16u, 0u, bgfx::TextureFormat::RGBA8);
                if (bgfx::invalidHandle == tex.idx)
                {
                    mix::Log::e("app", "failed to create CheckerTexture");
                }
            }

            void shutdown()
            {
                bgfx::destroyTexture (tex);
            }
        };

        Triangle tri;
        CheckerTexture checker;
        bgfx::ProgramHandle triProg;

        bgfx::ProgramHandle loadProgram (const char* _vsPath, const char* _fsPath)
        {
            mix::Buffer _buf;

            bgfx::ShaderHandle _vsh, _fsh;

            {
                mix::Result _ret = mix::Asset::load (_buf, _vsPath);
                if (_ret.isFail())
                {
                    mix::Log::e (getAppId(), "failed to load vertex shader: %s", _ret.why());
                    return BGFX_INVALID_HANDLE;
                }

                _vsh = bgfx::createShader (bgfx::copy (_buf.ptr(), _buf.size()));
            }

            {
                mix::Result _ret = mix::Asset::load (_buf, _fsPath);
                if (_ret.isFail())
                {
                    mix::Log::e (getAppId(), "failed to load fragment shader: %s", _ret.why());
                    return BGFX_INVALID_HANDLE;
                }

                _fsh = bgfx::createShader (bgfx::copy (_buf.ptr(), _buf.size()));
            }
            
            return bgfx::createProgram (_vsh, _fsh, true);
        }
        

        TheApplication()
        {
            setMainFrontendDesc (mix::FrontendDesc::descAuto);
        }

        ~TheApplication()
        {
        }

        const char* getAppId() const override
        {
            return "example_01";
        }
        
        mix::Result init() override
        {
            bgfx::setDebug (BGFX_DEBUG_TEXT);

            tri.init();
            checker.init();

            triProg = loadProgram ("shader/triangle_vs_main.sb", "shader/triangle_fs_main.sb");
            
            return mix::Result::ok();
        }
        
        void shutdown() override
        {
            bgfx::destroyProgram (triProg);
            tri.shutdown();
            checker.shutdown();
        }
        
        void update() override
        {
            bgfx::setViewRect (0, 0, 0, getMainFrontendDesc().width, getMainFrontendDesc().height);
            bgfx::touch (0);

            float t = getTimeSource().totalTimeInMS() * (1 / 128.0f);
            
            bgfx::setViewClear (0
                , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
                , 0x303030ff
                , 1.0f
                , 0
                );

            bgfx::dbgTextClear();
            bgfx::dbgTextPrintf (0, 1, 0x4f, "t = %.2f", t);

            tri.update (t);

            bgfx::setVertexBuffer (tri.vtxbuf);
            bgfx::setIndexBuffer (tri.idxbuf);

            bgfx::setState (BGFX_STATE_DEFAULT);
            
            bgfx::submit (0, triProg);
            bgfx::frame ();
        }

        void handleEvent (const mix::Event* _event)
        {
            const mix::FrontendEvent* _tevt = _event->cast<mix::FrontendEvent>();
            if (nullptr != _tevt)
            {
            }
        }
    };

    bgfx::VertexDecl TheApplication::Triangle::sharedVtxDecl;
    TheApplication* theApp = new TheApplication();
    
} // namespace example
