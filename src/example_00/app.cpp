#include <mix/mix_application.h>
#include <mix/mix_frontend.h>
#include <bgfx.h>
#include <math.h>

namespace example
{
    class TheApplication : public mix::Application
    {
    public:
        TheApplication()
        {
            setMainFrontendDesc (mix::FrontendDesc::descAuto);
        }

        ~TheApplication()
        {
        }

        const char* getAppId() const override
        {
            return "example_00";
        }
        
        mix::Result init() override
        {
            bgfx::setDebug (BGFX_DEBUG_TEXT|BGFX_DEBUG_STATS);
            
            return mix::Result::ok();
        }
        
        void shutdown() override
        {
            
        }
        
        void update() override
        {
            // r, g, b, a
            uint32_t _color[] = {
                0xff3030ff,
                0x30ff30ff,
                0x3030ffff,
                0xffffffff,
            };

            bgfx::touch (0);

            bgfx::setViewRect (0, 0, 0, getMainFrontendDesc().width, getMainFrontendDesc().height);

            int _i = (int)floorf (getTimeSource().totalTimeInMS() / 2048) % 4;

            bgfx::setViewClear (0
                , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
                , _color[_i]
                , 1.0f
                , 0
                );

            bgfx::frame ();
        }

        void handleEvent (const mix::Event* _event)
        {
            {
                const mix::FrontendEvent* _tevt = _event->cast<mix::FrontendEvent>();
                if (nullptr != _tevt)
                {
                    if (_tevt->type == mix::FrontendEventType::Resized)
                        mix::Log::i ("app", "Frontend Resized %d, %d", _tevt->params.size.w, _tevt->params.size.h);

                    if (_tevt->type == mix::FrontendEventType::Closed)
                         mix::Log::i ("app", "Frontend Closed");

                    if (_tevt->type == mix::FrontendEventType::TouchDown)
                         mix::Log::i ("app", "Frontend TouchDown %d; %f, %f", _tevt->touchid, _tevt->params.location.x, _tevt->params.location.y);

                    if (_tevt->type == mix::FrontendEventType::TouchMove)
                         mix::Log::i ("app", "Frontend TouchMove %d; %f, %f", _tevt->touchid, _tevt->params.location.x, _tevt->params.location.y);

                    if (_tevt->type == mix::FrontendEventType::TouchUp)
                         mix::Log::i ("app", "Frontend TouchUp %d; %f, %f", _tevt->touchid, _tevt->params.location.x, _tevt->params.location.y);

                    if (_tevt->type == mix::FrontendEventType::TouchCancel)
                         mix::Log::i ("app", "Frontend TouchCancel %d; %f, %f", _tevt->touchid, _tevt->params.location.x, _tevt->params.location.y);
                }
            }
            {
                const mix::ApplicationEvent* _tevt = _event->cast<mix::ApplicationEvent>();
                if (nullptr != _tevt)
                {
                    if (_tevt->type == mix::ApplicationEventType::Terminating)
                        mix::Log::i ("app", "Application Terminating");

                    if (_tevt->type == mix::ApplicationEventType::LowMemory)
                        mix::Log::i ("app", "Application LowMemory");

                    if (_tevt->type == mix::ApplicationEventType::WillEnterBackground)
                        mix::Log::i ("app", "Application WillEnterBackground");

                    if (_tevt->type == mix::ApplicationEventType::DidEnterBackground)
                        mix::Log::i ("app", "Application DidEnterBackground");

                    if (_tevt->type == mix::ApplicationEventType::WillEnterForeground)
                        mix::Log::i ("app", "Application WillEnterForeground");

                    if (_tevt->type == mix::ApplicationEventType::DidEnterForeground)
                        mix::Log::i ("app", "Application DidEnterForeground");
                }
            }
        }
    };

    TheApplication* theApp = new TheApplication();
    
} // namespace example
