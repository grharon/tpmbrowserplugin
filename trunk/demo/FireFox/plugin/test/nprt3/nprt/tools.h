#ifndef TOOLS_H
#define TOOLS_H
const char conf_file [] = "tpm_plugin.conf";
const char spawn_file [] = "jnicall.exe";
const char plugin_path [] = "plugins";

const char* getTemporaryPath();
const char* getHomePath();
const char* getFirefoxPath();
const char* getClassPath();
const char* getSpawnPath();
const char* getConfPath();

#endif