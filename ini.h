#include <filesystem>
#include <fstream>
#include <istream>
#include <string>
#include <cctype>

#include <bits/predefined_ops.h>

namespace makinami
{
	using namespace std::string_literals;

	struct config
	{
		std::unordered_map<std::string, std::unordered_map<std::string, std::string>> sections;
	};

	auto parse_section_name(std::string_view line) -> std::string_view
	{
		if (auto end = std::find(line.begin(), line.end(), ']'); end != line.end())
			return { line.begin() + 1, std::distance(line.begin(), end) - 1 };
		else
			throw std::runtime_error("Clonsing bracket of section name not found in "s + std::string(line));
	}

	auto trim_string_view(std::string_view value) -> std::string_view
	{
		auto begin = std::find_if(value.begin(), value.end(), [](auto c) { return !std::isspace(c); });
		auto end = std::find_if(value.rbegin(), value.rend(), [](auto c) { return !std::isspace(c); });

		return { begin, std::distance(begin, end.base())};
	}

	auto read_ini(std::istream& input) -> const config
	{
		auto temp_config = config{};
		auto current_section = ""s;

		std::string line;
		while (std::getline(input, line))
		{
			// skip comment lines
			if (line.at(0) == ';') continue;

			if (line.at(0) == '[')
			{
				current_section = parse_section_name(line);
				continue;
			}

			if (const auto equal = std::find(line.begin(), line.end(), '='); equal != line.end())
			{
				const auto key = trim_string_view(std::string_view(line).substr(0, std::distance(line.begin(), equal)));
				const auto value = trim_string_view(std::string_view(line).substr(std::distance(line.begin(), equal) + 1, line.length()));
				if (key.length() && value.length())
					temp_config.sections[current_section][std::string(key)] = value;
			}
		}

		return temp_config;
	}

	auto read_ini(const std::filesystem::path& file) -> const config
	{
		auto stream = std::ifstream(file);
		if (!stream.is_open())
			throw std::runtime_error("Could not open "s + file.c_str() + " for reading");
		return read_ini(stream);
	}
}