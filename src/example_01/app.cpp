#include <mix/mix_application.h>
#include <mix/mix_frontend.h>
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

            void shutdown()
            {
                bgfx::destroyDynamicVertexBuffer (vtxbuf);
                bgfx::destroyDynamicIndexBuffer (idxbuf);
            }
        };

        Triangle tri;
        bgfx::ProgramHandle triProg;

        bgfx::ProgramHandle loadProgram (const char* _vsPath, const char* _fsPath)
        {
            mix::Buffer _buf;

            bgfx::ShaderHandle _vs, _fs;

            {
                mix::Result _ret = loadAsset (_buf, _vsPath);
                if (_ret.isFail())
                {
                    mix::Log::e (getAppId(), "failed to load vs: %s", _ret.why());
                    return BGFX_INVALID_HANDLE;
                }

                _vs = bgfx::createShader (bgfx::copy (_buf.ptr(), _buf.size()));
            }

            {
                mix::Result _ret = loadAsset (_buf, _vsPath);
                if (_ret.isFail())
                {
                    mix::Log::e (getAppId(), "failed to load vs: %s", _ret.why());
                    return BGFX_INVALID_HANDLE;
                }

                _fs = bgfx::createShader (bgfx::copy (_buf.ptr(), _buf.size()));
            }
            
            return bgfx::createProgram (_vs, _fs, true);
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

            triProg = loadProgram ("shader/triangle_vs_main.sb", "shader/triangle_fs_main.sb");
            
            return mix::Result::ok();
        }
        
        void shutdown() override
        {
            bgfx::destroyProgram (triProg);
            tri.shutdown();
        }
        
        void update() override
        {
            bgfx::setViewRect (0, 0, 0, getMainFrontendDesc().width, getMainFrontendDesc().height);
            bgfx::touch (0);

            float t = floorf (fmodf (getTimeSource().totalTimeInMS() * 0.0625f, 128.0f));
            
            bgfx::setViewClear (0
                , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
                , 0x303030ff
                , 1.0f
                , 0
                );

            bgfx::dbgTextClear();
            bgfx::dbgTextPrintf (0, 1, 0x4f, "t = %.2f", t);
            
            //bgfx::submit (0);
            bgfx::frame ();
        }

        void handleEvent (const mix::Event* _event)
        {
            const mix::FrontendEvent* _tevt = _event->cast<mix::FrontendEvent>();
            if (nullptr != _tevt)
            {
                if (_tevt->type == mix::FrontendEventType::Resized)
                {
                    mix::Log::i ("app", "Frontend Resized %d, %d", _tevt->params.size.w, _tevt->params.size.h);
                    bgfx::reset (_tevt->params.size.w, _tevt->params.size.h, BGFX_RESET_NONE);
                }
            }
        }
    };
    bgfx::VertexDecl TheApplication::Triangle::sharedVtxDecl;
    TheApplication* theApp = new TheApplication();
    
} // namespace example
