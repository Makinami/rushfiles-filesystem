#include "client.h"

#include <exception>

#include <cpprest/http_client.h>

using namespace rushfiles::api::models;
using namespace std::string_literals;
using json = nlohmann::json;

std::vector<DomainTokenDto> rushfiles::client::login(std::string username, std::string password, std::string domain)
{
	const auto login_request = GetDomainToken{ username, password, "f28687ce4dcd4460bd6691f5657b1014" };

	auto client = web::http::client::http_client{ "https://clientgateway.cloudfile.jp" };
	
	return client.request(web::http::methods::POST, "/api/domaintokens", json(login_request).dump(), "application/json").then([=](web::http::http_response response)
	{
		return response.extract_string();
	}).then([=](std::string body) {
		auto response = json::parse(body).get<DomainTokensResponse>();
		if (!response.ResponseInfo && response.Data->DomainTokens)
			return response.Data->DomainTokens.value();
		else
			throw std::runtime_error("Failed to log in to RushFiles with message: "s + response.Message.value());
	}).get();
}

auto rushfiles::client::get_shares(const std::string& Username, const DomainTokenDto& token) -> std::vector<ShareRelationsAndAccessDto>
{
	auto client = web::http::client::http_client{ "https://clientgateway."s + *token.DomainUrl };
	
	auto request = web::http::http_request(web::http::methods::GET);
	request.set_request_uri("/api/users/"s + Username + "/shares");
	request.headers().add("Authorization", "DomainToken "s + *token.DomainToken);
	
	return client.request(request).then([=](web::http::http_response response) {
		return response.extract_string();
	}).then([=](std::string body) {
		auto response = json::parse(body).get<ShareRelationsAndAccessDtosResponse>();
		if (response.Message && *response.Message == "Ok.")
			return *response.Data;
		else
			throw std::runtime_error("Failed to get shares with message: "s + *response.Message);
	}).get();
}
