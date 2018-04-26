#include <exception>
template<class T>
class OutofBoundsException :public std::logic_error
{
public:
	OutofBoundsException(std::string name, std::string where): std::logic_error(name+" at line"+where)
	{}
	/*~OutofBoundsException(){
	}*/
	/*const char* what() const throw(){
		std::string temp;
		temp ="Out of bounds in "+name+" max value is "+std::to_string(max)+" given "+(std::to_string(given));
		/*char * writable = new char[temp.size() + 1];
		std::copy(temp.begin(), temp.end(), writable);
		writable[temp.size()] = '\0';*/
		
		/*const char* r= new char[temp.size()];
		*r = temp.c_str();
		return temp.c_str(); // This prly needs to be dynamic but I cannot figure out how to do that 
	}
	std::string getError(){
		std::string temp;
		temp ="Out of bounds in "+name+" max value is "+std::to_string(max)+" given "+(std::to_string(given));
		return temp;
	}*/
/*	
private:
	std::string name;
	std::string where;
	int max;
	int given;*/

};