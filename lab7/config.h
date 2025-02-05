#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

struct Config
{
    Config() = delete;

    struct Default {
        Default() = delete;

        static inline const bool password_correct = false;
    };
    static inline bool password_correct = Default::password_correct;
};

#endif // CONFIG_H
