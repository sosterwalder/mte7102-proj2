#include "main.hpp"

int main(int /* argc */, char ** /* argv */)
{
    try {
        spdlog::rotating_logger_st("qde", "/tmp/qde.log", 1048576 * 5, 1, true);
        spdlog::get("qde")->set_level(spdlog::level::debug);
        spdlog::get("qde")->info("Initializing GUI");
        nanogui::init();
        {
            nanogui::ref<Qce> qce = new Qce();
            qce->drawAll();
            qce->setVisible(true);
            nanogui::mainloop();
        }
        nanogui::shutdown();
    }
    catch (const std::runtime_error &e) {
        std::string error_msg = std::string("Caught a fatal error: ") + std::string(e.what());
        spdlog::get("qde")->error(error_msg);
        std::cerr << error_msg << endl;

        return -1;
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cerr << "Log failed: " << ex.what() << std::endl;
    }

    spdlog::get("qde")->info("Application finished");
    return 0;
}
