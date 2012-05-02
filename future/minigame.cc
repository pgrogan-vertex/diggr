
#include <sstream>

#include "mainloop.h"

#include "script.h"


struct Game {

    scripting::Vm vm;

    Game(bool verbose = false) : vm("piccol/", "future/scripts/", verbose) {}


    void init() {
        vm.init();
    }

    void generate() {

        vm.generate();
        redraw_skin();
    }

    void redraw_skin() {
        unsigned int gw = grender::get().w;
        unsigned int gh = grender::get().h;

        for (unsigned int y = 0; y < gh; ++y) {
            for (unsigned int x = 0; x < gw; ++x) {
                vm.set_skin(x, y);
            }
        }
    }

    void endgame() {
        std::cout << "Done!" << std::endl;
    }

    template <typename SINK>
    void save(SINK& s) {
        vm.save(s);
    }

    template <typename SOURCE>
    void load(SOURCE& s) {
        vm.load(s);
    }

    void drawing_context(mainloop::drawing_context_t& ctx) {
        vm.drawing_context(ctx);
    }

    void draw_hud() {
        vm.draw_hud();
    }

    void process_world(size_t& ticks, bool& done, bool& dead, bool& need_input) {

        //std::ostringstream ss;
        //ss << "Turn " << ticks;
        //grender::get().do_message(ss.str(), false);

        vm.process_world(ticks, done, dead, need_input);
    }

    void handle_input(size_t& ticks, bool& done, bool& dead, int vk, char c) {

        std::cout << "!" << ticks << " " << vk << " [" << c << "]" << std::endl;

        bool regen = false;
        bool redraw = false;

        vm.handle_input(ticks, vk, c, done, dead, regen, redraw);

        if (regen) {
            generate();

        } else if (redraw) {
            redraw_skin();
        }
    }
};

int main(int argc, char** argv) {

    mainloop::Main<Game> main;

    main.mainloop("_minisave.tmp", ::time(NULL));

    return 0;
}
