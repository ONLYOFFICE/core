#pragma once

#include <string>
#include <vector>
#include <map>
#include <iostream>

class HtmlFilePostProcessor
{
public:
    void AddReplacementDirective( std::wstring directive, std::wstring replacement )
    {
        m_aDirectives.insert( std::make_pair( directive, replacement ) );
    }

    void Process( std::wstring filename_in, std::wstring filename_out )
    {
        std::wfstream stream_in( filename_in.c_str() );
        std::wfstream stream_out( filename_out.c_str() );
        while ( !stream_in.eof() )
        {
        }
    }
private:
    std::map< std::wstring, std::wstring > m_aDirectives;
};