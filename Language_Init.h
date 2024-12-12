#pragma once

struct Language
{
    ONEGIN* onegin;
};

Language*  Language_init (int argc ,char* argv[]);