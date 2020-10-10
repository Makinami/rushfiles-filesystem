#include <string>
#include <unordered_map>
#include <optional>
#include <cstdint>

#include <nlohmann/json.hpp>

#include <iostream>

namespace {
	template <typename T, typename Enable = void>
	struct is_optional : std::false_type {};

	template <typename T>
	struct is_optional<std::optional<T>> : std::true_type {};
}

template <typename T>
inline void to_json_member(nlohmann::json& nlohmann_json_j, const char* key, const T& member)
{
	if constexpr (is_optional<T>::value)
	{
		if (member)
			nlohmann_json_j[key] = member.value();
	}
	else
	{
		nlohmann_json_j[key] = member;
	}
}

template <typename T>
inline void from_json_member(const nlohmann::json& nlohmann_json_j, const char* key, T& member)
{
	if constexpr (is_optional<T>::value)
	{
		auto it = nlohmann_json_j.find(key);
		if (it != nlohmann_json_j.end())
		{
			member = it->get<typename T::value_type>();
		}
	}
	else
	{
		nlohmann_json_j.at(key).get_to(member);
	}
}

#define JSON_TO_MEMBER(v1) to_json_member(nlohmann_json_j, #v1, nlohmann_json_t.v1);
#define JSON_FROM_MEMBER(v1) from_json_member(nlohmann_json_j, #v1, nlohmann_json_t.v1);

#define DEFINE_JSON_TYPE_WITH_OPTIONAL(Type, ...)  \
    inline void to_json(nlohmann::json& nlohmann_json_j, const Type& nlohmann_json_t) { NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(JSON_TO_MEMBER, __VA_ARGS__)) } \
    inline void from_json(const nlohmann::json& nlohmann_json_j, Type& nlohmann_json_t) { NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(JSON_FROM_MEMBER, __VA_ARGS__)) }

namespace rushfiles::api::models
{
	using std::int64_t;

	enum class ShareAssociationTypeEnum
	{
		Read = 0,
		Write = 1,
		Owner = 2
	};

	enum class AccessOriginEnum
	{
		User = 0,
		Group = 1
	};

	enum class ShareTypeEnum
	{
		Normal = 0,
		SubShare = 1
	};

	enum class ShareCategoryEnum
	{
		Corporate = 0,
		Team = 1,
		Private = 2
	};

	// RushFiles.ApiModel.Models.Dto.Base.ResponseInfo
	struct ResponseInfoDto
	{
		std::optional<int> ResponseCode;
		std::optional<std::string> Reference;
	};
	DEFINE_JSON_TYPE_WITH_OPTIONAL(ResponseInfoDto, ResponseCode, Reference);

	// RushFiles.ApiModel.Models.Dto.DomainToken.Request.GetDomainToken
	struct GetDomainToken
	{
		std::string UserName;
		std::string Password;
		std::string DeviceId;
		std::optional<double> Longitude;
		std::optional<double> Latitude;
		
		operator std::string() const { return nlohmann::json(*this).dump(); }
	};
	DEFINE_JSON_TYPE_WITH_OPTIONAL(GetDomainToken, UserName, Password, DeviceId,
		Longitude, Latitude);

	// RushFiles.ApiModel.Models.Dto.DomainToken.DomainTokenDto
	struct DomainTokenDto
	{
		std::optional<std::string> DomainUrl;
		std::optional<std::string> DomainToken;
	};
	DEFINE_JSON_TYPE_WITH_OPTIONAL(DomainTokenDto, DomainUrl, DomainToken);

	// RushFiles.ApiModel.Models.Dto.DomainToken.DomainTokenContainer
	struct DomainTokenContainer
	{
		std::optional<std::vector<DomainTokenDto>> DomainTokens;
		std::optional<bool> WipeDevice;
	};
	DEFINE_JSON_TYPE_WITH_OPTIONAL(DomainTokenContainer, DomainTokens, WipeDevice);

	// RushFiles.ApiModel.Models.Dto.DomainToken.Response.DomainTokensResponse
	struct DomainTokensResponse
	{
		std::optional<DomainTokenContainer> Data;
		std::optional<std::string> Message;
		std::optional<ResponseInfoDto> ResponseInfo;
	};
	DEFINE_JSON_TYPE_WITH_OPTIONAL(DomainTokensResponse, Data, Message, ResponseInfo);

	// RushFiles.ApiModel.Models.Dto.Catchup.Request.CatchupShare
	struct CatchupShare
	{
		std::optional<int> FromTick;
		std::optional<int> InitialTick;
		std::optional<int> BatchSize;
	};
	DEFINE_JSON_TYPE_WITH_OPTIONAL(CatchupShare, FromTick, InitialTick, BatchSize);

	// RushFiles.ApiModel.Models.Dto.ShareRelation.ShareRelationDto
	struct ShareRelationDto
	{
		std::optional<std::string> Id;
		std::optional<std::string> Name;
		std::optional<std::string> ProfilePicture;
		std::optional<std::string> Initials;
		std::optional<ShareAssociationTypeEnum> ShareAssociationType;
		std::optional<AccessOriginEnum> AccessOrigin;
	};
	DEFINE_JSON_TYPE_WITH_OPTIONAL(ShareRelationDto, Id, Name, ProfilePicture,
		Initials, ShareAssociationType, AccessOrigin);

	// RushFiles.ApiModel.Models.Dto.Share.ShareDefaultDto
	struct ShareDefaultDto
	{
		std::optional<std::string> Owner;
		std::optional<ShareCategoryEnum> ShareCategory;
	};
	DEFINE_JSON_TYPE_WITH_OPTIONAL(ShareDefaultDto, Owner, ShareCategory);

	// RushFiles.ApiModel.Models.Dto.Share.SpaceUsageDto
	struct SpaceUsageDto
	{
		std::optional<int64_t> DiskUsage;
		std::optional<int64_t> HistoryUsage;
		std::optional<int64_t> DeletedUsage;
	};
	DEFINE_JSON_TYPE_WITH_OPTIONAL(SpaceUsageDto, DiskUsage, HistoryUsage, DeletedUsage);

	// RushFiles.ApiModel.Models.Dto.TimeStampsDto
	struct TimeStampsDto
	{
		std::optional<int64_t> CreateTime;
		std::optional<std::string> CreatedBy;
		std::optional<int64_t> UpdateTime;
		std::optional<std::string> UpdatedBy;
		std::optional<int64_t> DeleteTime;
		std::optional<std::string> DeletedBy;
	};
	DEFINE_JSON_TYPE_WITH_OPTIONAL(TimeStampsDto, CreateTime, CreatedBy, UpdateTime,
		UpdatedBy, DeleteTime, DeletedBy);

	// RushFiles.ApiModel.Models.Dto.Share.ShareRelationsAndAccessDto
	struct ShareRelationsAndAccessDto
	{
		std::optional<ShareAssociationTypeEnum> ShareAssociationType;
		std::optional<std::vector<ShareRelationDto>> ShareRelations;
		std::optional<std::string> Id;
		std::optional<std::string> CompanyId;
		std::optional<std::string> VirtualFieldId;
		std::optional<std::string> Name;
		std::optional<ShareDefaultDto> ShareDefault;
		std::optional<int> ShareTick;
		std::optional<SpaceUsageDto> SpaceUsage;
		std::optional<ShareTypeEnum> ShareType;
		std::optional<std::vector<std::string>> ShareIds;
		std::optional<std::string> ParentShareId;
		std::optional<TimeStampsDto> TimeStamps;
	};
	DEFINE_JSON_TYPE_WITH_OPTIONAL(ShareRelationsAndAccessDto, ShareAssociationType,
		ShareRelations, Id, CompanyId, VirtualFieldId, Name, ShareDefault, ShareTick,
		SpaceUsage, ShareType, ShareIds, ParentShareId, TimeStamps);

	// RushFiles.ApiModel.Models.Dto.Share.Response.ShareRelationsAndAccessDtosResponse
	struct ShareRelationsAndAccessDtosResponse
	{
		std::optional<std::vector<ShareRelationsAndAccessDto>> Data;
		std::optional<std::string> Message;
		std::optional<ResponseInfoDto> ResponseInfo;
	};
	DEFINE_JSON_TYPE_WITH_OPTIONAL(ShareRelationsAndAccessDtosResponse, Data, Message,
		ResponseInfo);
}