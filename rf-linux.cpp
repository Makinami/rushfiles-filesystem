// rf-linux.cpp : Defines the entry point for the application.
//

#include "rf-linux.h"

#include <nlohmann/json.hpp>
#include <cpprest/http_client.h>
#include <fuse.h>
#include <fmt/core.h>

#include "ini.h"
#include "rushfiles/client.h"

using namespace std;

int main()
{
	auto config = makinami::read_ini(std::filesystem::path("settings.ini"));

	auto instances = rushfiles::client::login(config.sections["credentials"]["username"], config.sections["credentials"]["password"], "f28687ce4dcd4460bd6691f5657b1014");
	
	std::vector<rushfiles::api::models::ShareRelationsAndAccessDto> shares;
	for (const auto& instance : instances)
	{
		try
		{
			auto domain_shares = rushfiles::client::get_shares("wklonowski@tsukaeru.net", instance);
			shares.insert(shares.end(), std::make_move_iterator(domain_shares.begin()), std::make_move_iterator(domain_shares.end()));
		}
		catch (const std::runtime_error& exception)
		{
			fmt::print("Exception when retriving shares from {} domain: {}\n", *instance.DomainUrl, exception.what());
		}
	}

	for (const auto& share : shares)
		fmt::print("Share name: {} Share Id: {}\n", *share.Name, *share.Id);
	
	cin.get();
	return 0;
}
