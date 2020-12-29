#pragma once


template<EVariantTypes T>
FString FVariantToString(const FVariant& val)
{
	return TEXT("");
}

template<>
FString FVariantToString<EVariantTypes::Int8>(const FVariant& val)
{
	int32 value = (int32)val.GetValue<INT8>();
	return FString::FromInt(value);
}

template<>
FString FVariantToString<EVariantTypes::UInt8>(const FVariant& val)
{
	int32 value = (int32)val.GetValue<UINT8>();
	return FString::FromInt(value);
}

template<>
FString FVariantToString<EVariantTypes::Int16>(const FVariant& val)
{
	int32 value = (int32)val.GetValue<INT16>();
	return FString::FromInt(value);
}

template<>
FString FVariantToString<EVariantTypes::UInt16>(const FVariant& val)
{
	int32 value = (int32)val.GetValue<UINT16>();
	return FString::FromInt(value);
}

template<>
FString FVariantToString<EVariantTypes::Int32>(const FVariant& val)
{
	int32 value = val.GetValue<INT32>();
	return FString::FromInt(value);
}

template<>
FString FVariantToString<EVariantTypes::UInt32>(const FVariant& val)
{
	int32 value = (int32)val.GetValue<UINT32>();
	return FString::FromInt(value);
}

template<>
FString FVariantToString<EVariantTypes::Int64>(const FVariant& val)
{
	int64 value = val.GetValue<int64>();
	return FString::Printf(TEXT("%I64d"), value);
}

template<>
FString FVariantToString<EVariantTypes::UInt64>(const FVariant& val)
{
	int64 value = val.GetValue<uint64>();
	return FString::Printf(TEXT("%I64d"), value);
}

template<>
FString FVariantToString<EVariantTypes::Float>(const FVariant& val)
{
	float value = val.GetValue<float>();
	return FString::SanitizeFloat(value);
}

template<>
FString FVariantToString<EVariantTypes::String>(const FVariant& val)
{
	return TEXT("\"") + val.GetValue<FString>() + TEXT("\"");
}

FString GetStringFromVariant(const FVariant& Val)
{
	if (Val.GetType() == EVariantTypes::Int8)
	{
		return FVariantToString<EVariantTypes::Int8>(Val);
	}
	else if (Val.GetType() == EVariantTypes::UInt8)
	{
		return FVariantToString<EVariantTypes::UInt8>(Val);
	}
	else if (Val.GetType() == EVariantTypes::Int16)
	{
		return FVariantToString<EVariantTypes::Int16>(Val);
	}
	else if (Val.GetType() == EVariantTypes::UInt16)
	{
		return FVariantToString<EVariantTypes::UInt16>(Val);
	}
	else if (Val.GetType() == EVariantTypes::Int32)
	{
		return FVariantToString<EVariantTypes::Int32>(Val);
	}
	else if (Val.GetType() == EVariantTypes::UInt32)
	{
		return FVariantToString<EVariantTypes::UInt32>(Val);
	}
	else if (Val.GetType() == EVariantTypes::Int64)
	{
		return FVariantToString<EVariantTypes::Int64>(Val);
	}
	else if (Val.GetType() == EVariantTypes::UInt64)
	{
		return FVariantToString<EVariantTypes::UInt64>(Val);
	}
	else if (Val.GetType() == EVariantTypes::Float)
	{
		return FVariantToString<EVariantTypes::Float>(Val);
	}
	else if (Val.GetType() == EVariantTypes::String)
	{
		return FVariantToString<EVariantTypes::String>(Val);
	}
	else if (Val.GetType() == static_cast<EVariantTypes>(KBEngine::EKBEVariantTypes::VariantArray))
	{
		FString ret(TEXT("("));
		KBEngine::FVariantArray values = Val;
		for (FVariant fv_ : values)
		{
			EVariantTypes type = fv_.GetType();
			FString strValue = GetStringFromVariant(fv_);
			if (!ret.Equals(TEXT("(")))
			{
				ret += TEXT(",");
			}
			ret += strValue;
		}
		ret += TEXT(")");
		return ret;
	}
	else
	{
		return TEXT("");
	}
}


