#include <mix/mix_application.h>
#include <mix/mix_frontend.h>
#include <mix/mix_shader.h>
#include <math.h>

namespace example
{
    class TheApplication : public mix::Application
    {
    public:
        struct Quad
        {
            static bgfx::VertexDecl sharedVtxDecl;

            bgfx::DynamicVertexBufferHandle vtxbuf;
            bgfx::DynamicIndexBufferHandle idxbuf;
            uint16_t idxmem[6];
            float vtxmem[5 * 4];

            Quad()
                : vtxbuf (BGFX_INVALID_HANDLE)
                , idxbuf (BGFX_INVALID_HANDLE)
            {
            }

            mix::Result init()
            {
                
                if (0 == sharedVtxDecl.getStride())
                {
                    sharedVtxDecl.begin()
                        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
                        .end();
                }

                vtxbuf = bgfx::createDynamicVertexBuffer (bgfx::makeRef (vtxmem, sizeof (vtxmem)), sharedVtxDecl, BGFX_BUFFER_NONE);

                if (!bgfx::isValid (vtxbuf))
                    return mix::Result::fail ("failed to create vertex buffer");

                idxbuf = bgfx::createDynamicIndexBuffer (bgfx::makeRef (idxmem, sizeof (idxmem)), BGFX_BUFFER_NONE);

                if (!bgfx::isValid (idxbuf))
                    return mix::Result::fail ("failed to create index buffer");

                return mix::Result::ok();
            }

            void shutdown()
            {
                bgfx::destroyDynamicVertexBuffer (vtxbuf);
                bgfx::destroyDynamicIndexBuffer (idxbuf);
            }

            void update (float _t)
            {
                float _s = cosf (_t) * 0.5f + 0.5f;
                _s = 0.25f + 0.75f * _s;

                int _i;

                _i = 0;
                idxmem[_i++] = 0u;
                idxmem[_i++] = 1u;
                idxmem[_i++] = 2u;

                idxmem[_i++] = 3u;
                idxmem[_i++] = 2u;
                idxmem[_i++] = 1u;

                _i = 0;
                vtxmem[_i++] =-_s; vtxmem[_i++] = _s; vtxmem[_i++] = 0.5f; vtxmem[_i++] = 0.0f; vtxmem[_i++] = 1.0f;
                vtxmem[_i++] =-_s; vtxmem[_i++] =-_s; vtxmem[_i++] = 0.5f; vtxmem[_i++] = 0.0f; vtxmem[_i++] = 0.0f;
                vtxmem[_i++] = _s; vtxmem[_i++] = _s; vtxmem[_i++] = 0.5f; vtxmem[_i++] = 1.0f; vtxmem[_i++] = 1.0f;
                vtxmem[_i++] = _s; vtxmem[_i++] =-_s; vtxmem[_i++] = 0.5f; vtxmem[_i++] = 1.0f; vtxmem[_i++] = 0.0f;

                bgfx::updateDynamicVertexBuffer (vtxbuf, 0, bgfx::makeRef (vtxmem, sizeof (vtxmem)));
                bgfx::updateDynamicIndexBuffer  (idxbuf, 0, bgfx::makeRef (idxmem, sizeof (idxmem)));

            }
        };

        struct CheckerTexture
        {
            bgfx::TextureHandle tex;

            enum {
                Size = 32u,
            };

            CheckerTexture()
                : tex (BGFX_INVALID_HANDLE)
            {

            }

            mix::Result init()
            {
                tex = bgfx::createTexture2D (Size, Size, 0u, bgfx::TextureFormat::RGBA8);
                if (!bgfx::isValid (tex))
                {
                    return mix::Result::fail ("failed to create CheckerTexture");
                }

                return mix::Result::ok();
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
                        if (_y == 0 || _x == 0)
                            _pixels[_r * Size + _c] = 0xff000000;
                    }
                }

                bgfx::updateTexture2D (tex, 0, 0, 0, Size, Size, bgfx::makeRef (_pixels, sizeof (_pixels)));
            }
        };

        struct UnlitTexShader
        {
            bgfx::ProgramHandle program;
            bgfx::UniformHandle s_tex;

            mix::Result init()
            {
                program = mix::Shader::loadFromAsset ("displaytex");
                if (!bgfx::isValid (program))
                    return mix::Result::fail ("failed to load program");

                s_tex = bgfx::createUniform ("s_tex", bgfx::UniformType::Int1);
                return mix::Result::ok();
            }

            void shutdown()
            {
                bgfx::destroyUniform (s_tex);
                bgfx::destroyProgram (program);
            }
        };

        Quad m_quad;
        CheckerTexture m_tex;
        UnlitTexShader m_shader;

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

            m_quad.init();
            m_tex.init();
            m_shader.init();
            
            return mix::Result::ok();
        }
        
        void shutdown() override
        {
            m_shader.shutdown();
            m_quad.shutdown();
            m_tex.shutdown();
        }
        
        void update() override
        {
            bgfx::setViewRect (0, 0, 0, getMainFrontendDesc().width, getMainFrontendDesc().height);
            bgfx::touch (0);

            float _t = getTimeSource().totalTimeInMS() * (1 / 256.0f);
            
            bgfx::setViewClear (0
                , BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
                , 0x606060ff
                , 1.0f
                , 0
                );

            bgfx::dbgTextClear();
            bgfx::dbgTextPrintf (0, 1, 0x4f, "t = %.2f", _t);

            m_quad.update (_t);
            m_tex.update (_t / 8.0f);

            bgfx::setVertexBuffer (m_quad.vtxbuf);
            bgfx::setIndexBuffer (m_quad.idxbuf);
            bgfx::setTexture (0, m_shader.s_tex, m_tex.tex, BGFX_TEXTURE_MAG_POINT|BGFX_TEXTURE_MIN_POINT);

            bgfx::setState (BGFX_STATE_DEFAULT);
            
            bgfx::submit (0, m_shader.program);
            bgfx::frame ();
        }

        void handleEvent (const mix::Event* _event)
        {
            BX_UNUSED (_event);
        }
    };

    bgfx::VertexDecl TheApplication::Quad::sharedVtxDecl;
    TheApplication* theApp = new TheApplication();
    
} // namespace example
