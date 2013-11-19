#pragma once

class RtfToken
{
public: typedef enum {None ,Keyword,Control ,Text ,Eof,GroupStart ,GroupEnd} RtfTokenType;
public: RtfTokenType Type;
public: CStringA Key;
public: bool HasParameter;
public: int Parameter;

public: RtfToken()
    {
		Type = None;
        Key = "";
		HasParameter = false;
		Parameter = 0;
    }
};