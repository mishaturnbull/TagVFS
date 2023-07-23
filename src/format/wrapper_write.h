/* Wrapper-file creation headers & declarations.
 * AGPL-3.0 license
 * Initial rev Misha Turnbull 2023-07-23
 *
 * This file contains inclusions, defines, and declarations for the CLI utility
 * to create TagVFS wrapper files.
 */

#pragma once

int read_stdin_input(char* prompt, char* buf, char* maxsize);
int main(int argc, char** argv);

