# include <string.h>

# include "webserv/context.hpp"
# include "webserv/http.hpp"

static void __check_if_ready_to_remove_this_directory_(const std::string &dir)
{
	DIR* 			current;
	struct dirent*	ent;

	if ((current = opendir(dir.c_str())) == 0)
		throw 403;
	while ((ent = readdir(current)))
	{
		if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
			continue;
		if (ent->d_type == DT_DIR)
		{
			try {
				__check_if_ready_to_remove_this_directory_(dir + '/' + ent->d_name);
			}
			catch (const int error) {
				closedir(current);
				throw error;
			}
		}
		else if (access((dir + '/' + ent->d_name).c_str(), W_OK) < 0)
		{
			closedir(current);
			throw 403;
		}
	}
	closedir(current);
}

static void __remove_this_directory_(const std::string &dir)
{
	DIR* 			current;
	struct dirent*	ent;
	
	if ((current = opendir(dir.c_str())) == 0)
		throw 500;
	while ((ent = readdir(current)))
	{
		if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
			continue;
		if (ent->d_type == DT_DIR)
		{
			try {
				__remove_this_directory_(dir + '/' + ent->d_name);
			}
			catch (const int error) {
				closedir(current);
				throw error;
			}
		}
		else if (unlink((dir + '/' + ent->d_name).c_str()) < 0)
		{
			closedir(current);
			throw 500;
		}
	}
	closedir(current);
	if (rmdir(dir.c_str()) < 0)
		throw 500;
}

Client* HTTP::deleteMethodHandler(Client *client)
{
	Context::Directives::const_iterator	directive;
	Request								*request = client->getRequest();
	struct stat							pathInfo;
	
	// Check for redirection
	directive = request->location->getDirectives().find(REDIRECT_DIRECTIVE);
	if (directive != request->location->getDirectives().end()) {
		throw std::make_pair(std::strtol(directive->second[0].c_str(), NULL, 10), directive->second[1]);
	}
	// check whatever file or folder exist to remove it
	if (stat(request->fullPath.c_str(), &pathInfo) < 0) {
		throw 404;
	}
	// Check is directory
	if (S_ISDIR(pathInfo.st_mode)) {
		if (request->path.c_str()[request->path.size()-1] != '/') {
			throw std::make_pair(301L, request->path + '/');
		} else {
			// remove hole directory
			request->fullPath.erase(request->fullPath.size() - 1);
			// request->fullPath.pop_back();
			__check_if_ready_to_remove_this_directory_(request->fullPath);
			__remove_this_directory_(request->fullPath);
		}
	} // this is a file, so remove it
	else if (unlink(request->fullPath.c_str()) < 0) {
		throw 403;
	}
	
	client->setResponse(new Response(200, request->keepAlive));
	client->getResponse()->addBody("Resource deleted successfully");
	return 0;
}
