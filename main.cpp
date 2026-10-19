#include "Application.cpp"

int main() {
    try {
        Application application;
        application.executer();
    } catch (const std::exception& error) {
        std::cerr << "Erreur fatale : " << error.what() << '\n';
        return 1;
    }
    return 0;
}
