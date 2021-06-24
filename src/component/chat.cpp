#include <stdinc.hpp>

#include "loader/component_loader.hpp"
#include "utils/hook.hpp"
#include "utils/string.hpp"

#include "chat.hpp"
#include "dvars.hpp"

namespace chat
{
    namespace
    {
        utils::hook::detour client_ui_hook;

        void client_ui_stub(int entRef, const char* command)
        {
            if (utils::string::starts_with(command, "say") && !dvars::sv_EnableGameChat->current.enabled)
            {
                const char* debug = utils::string::va("tell %d You are not allowed to type in the chat", entRef);
                game::Cbuf_AddText(0, debug);
                return;
            }

            client_ui_hook.invoke<void>(entRef, command);
        }
    }

    class component final : public component_interface
    {
    public:
        void post_unpack() override
        {
            client_ui_hook.create(0x0502CB0, &client_ui_stub);
        }
    };
}

REGISTER_COMPONENT(chat::component)
