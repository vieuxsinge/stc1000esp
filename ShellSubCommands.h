#pragma once

#include <Shell.h>

class ShellSubCommand {
  public:
    ShellSubCommand(const char* name, const char* help, bool(*func)(Shell &shell, int argc, const char* const argv[]));
    const char* name;
    const char* help;
    bool (*execute)(Shell &shell, int argc, const char* const argv[]);
};

class ShellSubCommands {
  public:
    ShellSubCommands(std::vector<ShellSubCommand> subcommands);
    void printHelp(Stream &s, const char* command);
    void execute(Shell &shell, int argc, const char* const argv[]);
  private:
    std::vector<ShellSubCommand> _subcommands;
};

