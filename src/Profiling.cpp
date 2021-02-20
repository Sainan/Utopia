#include "Profiling.hpp"

#include "config.hpp"

#include <chrono>
#include <iostream>
#include <string>
#include <unordered_map>

namespace Utopia
{
	static std::unordered_map<std::string, std::chrono::steady_clock::time_point> section_starts;

	void Profiling::startSection(const char* section)
	{
		section_starts.emplace(section, std::chrono::steady_clock::now());
	}

	void Profiling::endSection(const char* section)
	{
		std::string ns_str = std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - section_starts.at(section)).count());
		std::string section_str(section);
		if (section_str.length() < 32)
		{
			section_str.append(32 - section_str.length(), ' ');
		}
		if (ns_str.length() < 12)
		{
			ns_str.insert(0, 12 - ns_str.length(), ' ');
		}
		std::cout << section_str << ":" << ns_str << " ns" << std::endl;
	}
}
