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

            enum {
                Size = 16u,
            };

            CheckerTexture()
                : tex (BGFX_INVALID_HANDLE)
            {

            }

            void init()
            {
                tex = bgfx::createTexture2D (Size, Size, 0u, bgfx::TextureFormat::RGBA8);
                if (bgfx::invalidHandle == tex.idx)
                {
                    mix::Log::e("app", "failed to create CheckerTexture");
                }
            }

            void shutdown()
            {
                bgfx::destroyTexture (tex);
            }

            void update (float _t)
            {
                static uint32_t _pixels[Size * Size];

                // a, b, g, r
                uint32_t _baseclr[] = {
                    0xff303030,
                    0xffff3030,
                    0xff30ff30,
                    0xff3030ff,
                };

                uint32_t _gridclr[] = {
                    0x00ffffff,
                    _baseclr[((int)_t) % 4],
                };

                for (int _r = 0; _r < Size; ++_r)
                {
                    int _y = _r / 4;

                    for (int _c = 0; _c < Size; ++_c)
                    {
                        int _x = _c / 4;
                        _pixels[_r * Size + _c] = _gridclr[(_y + _x) % 2];
                    }
                }

                bgfx::updateTexture2D (tex, 0, 0, 0, Size, Size, bgfx::makeRef (_pixels, sizeof (_pixels)));
            }
        };

        Triangle tri;
        CheckerTexture checker;
        bgfx::ProgramHandle triProg;
        bgfx::UniformHandle texUni;

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

            triProg = loadProgram ("shader/displaytex_vs_main.sb", "shader/displaytex_fs_main.sb");
            texUni = bgfx::createUniform ("s_tex", bgfx::UniformType::Int1);
            
            return mix::Result::ok();
        }
        
        void shutdown() override
        {
            bgfx::destroyUniform (texUni);
            bgfx::destroyProgram (triProg);
            tri.shutdown();
            checker.shutdown();
        }
        
        void update() override
        {
            bgfx::setViewRect (0, 0, 0, getMainFrontendDesc().width, getMainFrontendDesc().height);
            bgfx::touch (0);

            float t = getTimeSource().totalTimeInMS() * (1 / 256.0f);
            
            bgfx::setViewClear (0
                , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
                , 0x606060ff
                , 1.0f
                , 0
                );

            bgfx::dbgTextClear();
            bgfx::dbgTextPrintf (0, 1, 0x4f, "t = %.2f", t);

            tri.update (t);
            checker.update (t / 8.0f);

            bgfx::setVertexBuffer (tri.vtxbuf);
            bgfx::setIndexBuffer (tri.idxbuf);
            bgfx::setTexture (0, texUni, checker.tex, BGFX_TEXTURE_MAG_POINT|BGFX_TEXTURE_MIN_POINT);

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
