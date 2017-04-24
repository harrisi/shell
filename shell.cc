#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __unix__
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#endif

#include <map>
#include <string>
#include <iostream>
using namespace std;

// TODO: Consider adding argument parsing to class.
class arguments { 
public:
	arguments(int argc, const char *const argv[])
		: argc_(argc), argv_(argv)
	{ }

	const char *const *begin() const { return argv_; }
	const char *const *end() const { return argv_ + argc_; }

private:
	const int argc_;
	const char *const *argv_;
};

class environment {
public:
	environment()
	{ }

	string &operator [] (const string &key);
private:
	map<string, string> vars;
};

string &environment::operator [] (const string &key)
{
	return vars[key];
}

// TODO: Consider creating a class to handle environment variables.
// TODO: Consider moving to C.

// TODO: Lacking specific information otherwise, the shell environment will be
// cloned when passed to another process.
// TODO: Error handling.
// TODO: Determine storage requirements of environment.
// TODO: Determine a possible replacement for environment variables.
// TODO: Pass `environ` in explicitly?
char *const *
clone_environ()
{
	char **env;
	int nenv = 0;

	for (char *const *penv = environ; *penv; penv++)
		nenv++;

	env = new char *[nenv + 1];
	memset(env, 0, nenv + 1);

	// TODO: Use safe string functions. Possibly avoid library functions.
	for (int ivar = 0; ivar < nenv; ivar++)
		env[ivar] = strdup(environ[ivar]);

	return env;
}

void
destroy_environ(char *const *env)
{
	int nenv = 0;
	for (char * const *penv = env; *penv; penv++) {
		free(*penv);
		nenv++;
	}
	delete[] env;
}


// TODO: Cross-platform support.
// TODO: Per-platform source files?
// TODO: Accept a single string?
#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
int
spawn(const string &path, const string &args)
{
	pid_t child = fork();

	// TODO: Make caller fill this in.
	char *const argv[] = {(char *)"", nullptr};
	// Is this freed properly?
	char *const *envp  = clone_environ();

	if (child == -1) {
		return -1;
	}
	else if (child) {
		destroy_environ(envp);	
		return child;
	} 
	else {
		if (execve(path.c_str(), argv, envp))
			// TODO: Use strerror_r.
			cout << "execve: " << strerror(errno) << std::endl;
	}
	return 0;
}
#endif

#if defined(_WIN32)
int
spawn(const string &path, const string &args)
{
	return 0;
}
#endif

// TODO: Cross-platform support.
// TODO: Determine which subset of features is shared.
int
join(int pid, int *status)
{
	return -1;
}

int 
main(int argc, char *argv[]) 
{
	int ls = spawn("/bin/ls", "");
	int status;

	if (join(ls, &status) == -1)
		// TODO: Use strerror_r.
		cout << "join: " << strerror(errno) << std::endl;

	return 0;
}
