#include "ShellSubCommands.h"

#include "ShellHelpers.h"

ShellSubCommand::ShellSubCommand(const char* n, const char* h, bool(*func)(Shell &shell, int argc, const char* const argv[])) {
  name = n;
  help = h;
  execute = func;
}


ShellSubCommands::ShellSubCommands(std::vector<ShellSubCommand> subcommands) {
  _subcommands = subcommands;
}

void ShellSubCommands::printHelp(Stream &s, const char* command) {
  s.printf("%s <subcommand> [...]\r\n", command);
  s.println("Available subcommands:");
  for(auto sc : _subcommands) {
    s.printf("- %s %s\r\n", sc.name, sc.help);
  }
}

void ShellSubCommands::execute(Shell &shell, int argc, const char* const argv[]) {
  if(!shellCheckArgs(shell, argc, argv, 2)) {
    printHelp(shell, argv[0]);
    return;
  }
  
  auto name = argv[1];
  auto subcommand = std::find_if(_subcommands.begin(), _subcommands.end(), [name](ShellSubCommand& sc){
    return strcmp(sc.name, name) == 0;
  });

  if(subcommand == _subcommands.end()) {
    shell.printf("Error: subcommand '%s' not found\r\n", name);
    printHelp(shell, argv[0]);
    return;
  }

  if( !(*subcommand).execute(shell, argc-1, argv+1) ) {
    printHelp(shell, argv[0]);
    return;
  }
}

