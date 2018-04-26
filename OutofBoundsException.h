#include <exception>
template<class T>
class OutofBoundsException :public exception
{
public:
	OutofBoundsException(std::string name, int max, int given){
		this->name = name;
		this->max = max;
		this->given = given;
	}
	~OutofBoundsException(){
	}
	const char* what() const throw(){
		std::string temp;
		temp ="Out of bounds in "+name+" max value is "+std::to_string(max)+" given "+(std::to_string(given));
		/*char * writable = new char[temp.size() + 1];
		std::copy(temp.begin(), temp.end(), writable);
		writable[temp.size()] = '\0';*/
		
		/*const char* r= new char[temp.size()];
		*r = temp.c_str();*/
		return temp.c_str(); // This prly needs to be dynamic but I cannot figure out how to do that 
	}
	std::string getError(){
		std::string temp;
		temp ="Out of bounds in "+name+" max value is "+std::to_string(max)+" given "+(std::to_string(given));
		return temp;
	}
	
private:
	std::string name;
	int max;
	int given;

};