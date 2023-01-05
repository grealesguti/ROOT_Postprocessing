#include <ctime>
#include <iomanip>
#include <string>

std::string getCurrentDateTimeString()
{
    // Get current date and time
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);

    // Format date and time as string
    std::ostringstream dateTime;
    dateTime << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
    return dateTime.str();
}
