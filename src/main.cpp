#include <Proxy/WeatherProxy.hpp>
#include <Cache/LRUCache.hpp>

int main() {
    auto cache = std::make_unique<LRUCache<std::string, nlohmann::json>>(100);
    WeatherProxy weather_proxy(std::move(cache));

    try {
        const auto data = weather_proxy.query("London");

        std::cout << "Temperature: " << data["days"][0]["temp"] << "°C\n";
        std::cout << "Conditions: " << data["days"][0]["conditions"] << "\n";
        std::cout << "Sunrise: " << data["days"][0]["sunrise"] << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
