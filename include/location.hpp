# ifndef LOCATION_HPP
# define LOCATION_HPP

/**
 * class Location - represents a location block in a server block
 */

# include <string>
# include <vector>

class Location {
    private :
        std::string match;
        std::string index;
        std::string root;
    public :
        // constructor
        Location(const std::vector<std::string>& contextArgs);

        // getters
        const std::string& getMatch() const;
        const std::string& getIndex() const;
        const std::string& getRoot() const;

        // setters
        void setIndex(const std::vector<std::string>& directiveArgs);
        void setRoot(const std::vector<std::string>& directiveArgs);
        
        // destructor
        ~Location();
};

# endif