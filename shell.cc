#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: Add BSDs.
#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#	define POSIX
#elif defined(_WIN32)
#	define WINAPI
#endif

#if defined(POSIX)
#	include <errno.h>
#	include <unistd.h>
#	include <sys/wait.h>
// TODO: How best to support unicode Windows calls?
#elif defined(WINAPI)
#	include <windows.h>
#endif

#include <map>
#include <string>
#include <iostream>
using namespace std;

// TODO: Consider adding argument parsing to class.
// TODO: Consider using gengetopt.
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

#if defined(POSIX)
typedef char ** environment_t;
#elif defined(WINAPI)
typedef char *  environment_t;
#endif

// TODO: Read in from OS format, modify, then pack into OS format for process
// creation.
// TODO: If or when moving to C, the best internal format is likely a null
// terminated array of strings.
class environment {
public:
	environment()
	{ this->from_environment(); }
	environment(const map<string, string> &vars)
		: vars_(vars)
	{ }

	void from_environment();
	environment_t to_environment();
	string &operator [] (const string &key);

private:
	map<string, string> vars_;
};

void
environment::from_environment()
{
#if defined(POSIX)
	const char *eq;
	string key, val;

	for (char *const *penv = environ; *penv; penv++) {
		eq  = strchr(*penv, '=');
		key = string(*penv, eq - *penv);
		val = string(eq + 1);

		vars_[key] = val;
	}
#elif defined(WINAPI)
	const char *env, *penv, *eq;
	string key, val;
	
	env  = GetEnvironmentStrings();
	penv = env;
	while (*penv) {
		eq  = strchr(penv, '=');
		key = string(penv, eq - penv);
		val = string(eq + 1);
		
		vars_[key] = val;
		penv = strchr(penv, '\0') + 1;
	}
	FreeEnvironmentStrings((char *)env);
#endif
}

environment_t
environment::to_environment()
{
#if defined(POSIX)
	char **env = new char *[vars_.size()],
		 **penv = env;

	for (const auto &v: vars_) {
		*penv = strdup((v.first + "=" + v.second).c_str());
		penv++;
	}
	return env;
#elif defined(WINAPI)
	char *env, *penv;
	string ent;
	int tenv = 0;
	
	// TODO: Find a better way to do this, if possible.
	for (const auto &v: vars_)
		// Size of key, value, '=', and '\0'.
		tenv += v.first.size() + v.second.size() + 2;
	// Size of all entries plus '\0'.
	env  = new char[tenv + 1];
	penv = env;
	memset(env, 0, tenv + 1);
	
	for (const auto &v: vars_) {
		ent = v.first + "=" + v.second;
		memcpy(penv, ent.c_str(), ent.size() + 1);
		penv += ent.size() + 1;
	}
	return env;
#endif
}

string &
environment::operator [] (const string &key)
{
	return vars_[key];
}

class process {
	process(const string &path, const string &args)
		: path_(path), args_(args)
	{ }

private:
	string path_, args_;
};

// TODO: Find a good way to handle the difference in environment variable
// formats.
// TODO: Lacking specific information otherwise, the shell environment will be
// cloned when passed to another process.
// TODO: Error handling.
// TODO: Determine storage requirements of environment.
// TODO: Determine a possible replacement for environment variables.
// TODO: Pass `environ` in explicitly?
// TODO: Cross-platform support.
// TODO: Per-platform source files?
// TODO: Accept a single string?
#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
int
spawn(const string &path, const string &args)
{
	pid_t child;
	environment env;

	// TODO: Make caller fill this in.
	char *const argv[] = {(char *)"", nullptr};
	// Is this freed properly?
	char *const *envp  = env.to_environment();

	child = vfork();
	if (child == -1) {
		return -1;
	}
	else if (child) {
		//destroy_environ(envp);	
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
// TODO: Determine which subset of features are shared.
int
join(int pid, int *status)
{
	return -1;
}

// TODO: Consider moving to C.
int 
main(int argc, char *argv[]) 
{
	//int ls = spawn("/bin/ls", "");
	//(void)ls;

	environment env;
	env.to_environment();
	
	return 0;
}
