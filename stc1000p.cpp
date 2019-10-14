#include "stc1000p.h"

#include <Stc1000p.h>
#include <Shell.h>
#include "ShellHelpers.h"
#include "ShellSubCommands.h"

Stc1000p stc1000p = Stc1000p(D0, INPUT_PULLDOWN_16);

void shellPrint(Stream &s, Stc1000pRunMode v) {
  switch(v) {
    case Stc1000pRunMode::TH:
      s.println("th");
      break;
    case Stc1000pRunMode::PR0:
      s.println("pr0");
      break;
    case Stc1000pRunMode::PR1:
      s.println("pr1");
      break;
    case Stc1000pRunMode::PR2:
      s.println("pr2");
      break;
    case Stc1000pRunMode::PR3:
      s.println("pr3");
      break;
    case Stc1000pRunMode::PR4:
      s.println("pr4");
      break;
    case Stc1000pRunMode::PR5:
      s.println("pr5");
      break;
  }
}

bool shellParse(const char* str, Stc1000pRunMode* v) {
  if( strcmp(str, "th") == 0 ) {
    *v = Stc1000pRunMode::TH;
    return true;
  }
  else if( strcmp(str, "pr0") == 0 ) {
    *v = Stc1000pRunMode::PR0;
    return true;
  }
  else if( strcmp(str, "pr1") == 0 ) {
    *v = Stc1000pRunMode::PR1;
    return true;
  }
  else if( strcmp(str, "pr2") == 0 ) {
    *v = Stc1000pRunMode::PR2;
    return true;
  }
  else if( strcmp(str, "pr3") == 0 ) {
    *v = Stc1000pRunMode::PR3;
    return true;
  }
  else if( strcmp(str, "pr4") == 0 ) {
    *v = Stc1000pRunMode::PR4;
    return true;
  }
  else if( strcmp(str, "pr5") == 0 ) {
    *v = Stc1000pRunMode::PR5;
    return true;
  }
  return false;
}

static auto subcommands = ShellSubCommands({

  ShellSubCommand("t", "Get Current temperature", [](Shell &shell, int argc, const char* const argv[]){
    return shellGet<float>(shell, argc, argv, [](float* v){
      return stc1000p.readTemperature(v);
    });
  }),

  ShellSubCommand("c", "Get Cooling status", [](Shell &shell, int argc, const char* const argv[]){
    return shellGet<bool>(shell, argc, argv, [](bool* v){
      return stc1000p.readCooling(v);
    });
  }),

  ShellSubCommand("h", "Get Heating status", [](Shell &shell, int argc, const char* const argv[]){
    return shellGet<bool>(shell, argc, argv, [](bool* v){
      return stc1000p.readHeating(v);
    });
  }),

  ShellSubCommand("sp", "Get/Set Setpoint", [](Shell &shell, int argc, const char* const argv[]){
    return shellGetSet<float>(shell, argc, argv, [](float* v){
      return stc1000p.readSetpoint(v);
    }, [](float v){
      return stc1000p.writeSetpoint(v);
    });
  }),

  ShellSubCommand("hy", "Get/Set Hysteresis", [](Shell &shell, int argc, const char* const argv[]){
    return shellGetSet<float>(shell, argc, argv, [](float* v){
      return stc1000p.readHysteresis(v);
    }, [](float v){
      return stc1000p.writeHysteresis(v);
    });
  }),

  ShellSubCommand("tc", "Get/Set Temperature correction", [](Shell &shell, int argc, const char* const argv[]){
    return shellGetSet<float>(shell, argc, argv, [](float* v){
      return stc1000p.readTemperatureCorrection(v);
    }, [](float v){
      return stc1000p.writeTemperatureCorrection(v);
    });
  }),

  ShellSubCommand("sa", "Get/Set Setpoint alarm", [](Shell &shell, int argc, const char* const argv[]){
    return shellGetSet<float>(shell, argc, argv, [](float* v){
      return stc1000p.readSetpointAlarm(v);
    }, [](float v){
      return stc1000p.writeSetpointAlarm(v);
    });
  }),

  ShellSubCommand("st", "Get/Set Current Profile Step", [](Shell &shell, int argc, const char* const argv[]){
    return shellGetSet<int>(shell, argc, argv, [](int* v){
      return stc1000p.readCurrentStep(v);
    }, [](int v){
      return stc1000p.writeCurrentStep(v);
    });
  }),

  ShellSubCommand("dh", "Get/Set Current Profile Duration", [](Shell &shell, int argc, const char* const argv[]){
    return shellGetSet<int>(shell, argc, argv, [](int* v){
      return stc1000p.readCurrentDuration(v);
    }, [](int v){
      return stc1000p.writeCurrentDuration(v);
    });
  }),

  ShellSubCommand("cd", "Get/Set Cooling Delay", [](Shell &shell, int argc, const char* const argv[]){
    return shellGetSet<int>(shell, argc, argv, [](int* v){
      return stc1000p.readCoolingDelay(v);
    }, [](int v){
      return stc1000p.writeCoolingDelay(v);
    });
  }),

  ShellSubCommand("hd", "Get/Set Heating Delay", [](Shell &shell, int argc, const char* const argv[]){
    return shellGetSet<int>(shell, argc, argv, [](int* v){
      return stc1000p.readHeatingDelay(v);
    }, [](int v){
      return stc1000p.writeHeatingDelay(v);
    });
  }),

  ShellSubCommand("rp", "Get/Set Ramping", [](Shell &shell, int argc, const char* const argv[]){
    return shellGetSet<bool>(shell, argc, argv, [](bool* v){
      return stc1000p.readRamping(v);
    }, [](bool v){
      return stc1000p.writeRamping(v);
    });
  }),

  ShellSubCommand("rn", "Get/Set Run Mode", [](Shell &shell, int argc, const char* const argv[]){
    return shellGetSet<Stc1000pRunMode>(shell, argc, argv, [](Stc1000pRunMode* v){
      return stc1000p.readRunMode(v);
    }, [](Stc1000pRunMode v){
      return stc1000p.writeRunMode(v);
    });
  }),

  ShellSubCommand("psp", "Get/Set Setpoint of Profile X Step Y", [](Shell &shell, int argc, const char* const argv[]){
    if( !shellCheckArgs(shell, argc, argv, 3, 4) ) {
      return false;
    }

    int profile = atoi(argv[1]);
    int step = atoi(argv[2]);
    
    return shellGetSet<float>(shell, argc-2, argv+2, [profile, step](float* v){
      return stc1000p.readProfileSetpoint(profile, step, v);
    }, [profile, step](float v){
      return stc1000p.writeProfileSetpoint(profile, step, v);
    });
  }),
  
  ShellSubCommand("pdh", "Get/Set Duration of Profile X Step Y", [](Shell &shell, int argc, const char* const argv[]){
    if( !shellCheckArgs(shell, argc, argv, 3, 4) ) {
      return false;
    }

    int profile = atoi(argv[1]);
    int step = atoi(argv[2]);
    
    return shellGetSet<int>(shell, argc-2, argv+2, [profile, step](int* v){
      return stc1000p.readProfileDuration(profile, step, v);
    }, [profile, step](int v){
      return stc1000p.writeProfileDuration(profile, step, v);
    });
  })

});

ShellCommand(stc1000p, "Access to STC1000+ parameters", [](Shell &shell, int argc, const ShellArguments &argv){
  subcommands.execute(shell, argc, argv.argv);
});

