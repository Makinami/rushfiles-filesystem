#include <vector>
#include <string>

#include "models.h"

namespace rushfiles::client
{
	using namespace rushfiles::api::models;

	auto login(std::string username, std::string password, std::string domain = "") -> std::vector<DomainTokenDto>;

	auto get_shares(const std::string& Username, const DomainTokenDto& token) -> std::vector<ShareRelationsAndAccessDto>;
}