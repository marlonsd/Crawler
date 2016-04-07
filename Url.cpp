#include "Url.h"

// Constructors

Url::Url(){
	this->size = 0;
}

Url::Url(char* url){
	this->url = this->canonicalizeUrl(url);
	this->size = this->getURLsize(this->url);
}

Url::Url(string url){
	this->url = this->canonicalizeUrl(url);
	this->size = this->getURLsize(this->url);
}

Url::Url(CkString url){
	// this->url = (char*) url.getString();
	this->url = this->canonicalizeUrl(url.getString());
	this->size = this->getURLsize(this->url);
}

Url::Url(Url const& url){
	this->url = url.getUrl();
	this->size = url.getSize();
}

// Destructor

Url::~Url(){
	// this->size = 0;
	// delete(&list);
	// del(list); // ?
}

int Url::getURLsize(string url){
	CkString canonicalized_url, domain_url, clean_url, path_url, aux_ckstr;
	string aux_str;
	CkStringArray *aux;
	int domain_size = 0, size = 0;

	canonicalized_url = this->canonicalizeUrl(url).c_str();
	
	// printf("\tURL: %s\n", url.getString());
	// printf("\tCanonicalized URL: %s\n", canonicalized_url.getString());

	clean_url = this->cleaningURL((char*) canonicalized_url.getString());
	// cout << "\tClean URL: " << clean_url.getString() << endl;

	// Check if there is something after "http[s]?://""
	if (clean_url.getNumChars() <= 0){
		return 0;
	}

	// 
	// /* Separating path */
	// aux_str = string(clean_url.getString());
	// aux_str = aux_str.erase(0,string(domain_url).size()+1);

	// cout << "\tPath: " << aux_str << endl;
	// 

	// Counting path's size (www.ufmg.br -> size = 3)	
	aux = clean_url.split('/', true, true, false);
	size += aux->get_Count()-1; // -1 because it is ignoring the domain, which will be considered later

	// Counting domain's size (www.ufmg.br -> size = 3)
	aux->StrAt(0, aux_ckstr);
	aux = aux_ckstr.split('.', true, true, false);
	size += aux->get_Count();

	// printf("\tURL size: %d\n\n", size);


	return size;

}

// Remove "http[s]?://" from url

// CkString chop

char* Url::cleaningURL(string url){
	string http ("http");
	string delimitation ("://");
	string new_url = url;
	size_t found = url.find(http); // Locate the position where "http" starts in the url

	// Test if "http" is within the url
	if (found!=std::string::npos){
		// Teste if "http" starts in the beginning of the url
		if (!found){
			// Locate the position where "://" starts in the url
			found = url.find(delimitation, http.size());
			// Tests if "://" is within the url
			if (found!=std::string::npos){
				// Remove "http[s]?://" from the url
				new_url = url.erase(0,found+delimitation.size());
			}
		}

	}

	// Converting string to char*
	char* final_url = new char[new_url.size()+1];
	std::copy(new_url.begin(), new_url.end(), final_url);
	final_url[new_url.size()] = '\0';

	return final_url;
}

// CkString Url::canonicalizeUrl(string url){
//Also removes "www."
string Url::canonicalizeUrl(string url){
	CkString new_url;
	CkSpider spider;

	spider.CanonicalizeUrl(url.c_str(), new_url); // Canonicalizing URL

	return ((char*) new_url.getString());

}


void Url::setUrl(char* url){
	this->url = this->canonicalizeUrl(url);
	// cout << "Url: " << this->url << endl;
	this->size = this->getURLsize(this->url);
}

void Url::setUrl(string url){
	this->url = this->canonicalizeUrl(url);
	// cout << this->url << endl;
	this->size = this->getURLsize(this->url);
	// cout << "\tWord In: " << this->url << endl;
}

void Url::setUrl(const char* url){
	// size_t len = strlen(url);
	// this->url = new char[len+1];
	// strncpy(this->url, url, len);
	// this->url[len] = '\0';
	// this->size = this->getURLsize(url);
	// this->url = url;
	this->url = this->canonicalizeUrl(url);
	this->size = this->getURLsize(this->url);
}

void Url::setUrl(CkString url){
	// this->url = (char*) url.getString();
	this->url = this->canonicalizeUrl(url.getString());
	this->size = this->getURLsize(this->url);
	// cout << "\tWord In: " << this->url << endl;
}

string Url::getUrl(){
	return this->url;
}

string Url::getUrl() const {
	return this->url;
}

string Url::getCleanUrl(){
	string url = this->cleaningURL(this->url);

	return url;
}

int Url::getSize(){
	return (int) size;
}

int Url::getSize() const {
	return (int) size;
}

string Url::getNormalizedUrl(){
	string url = this->canonicalizeUrl(this->url);
	string delimitation ("www.");

	size_t found = url.find(delimitation); // Locate the position where "www." starts in the url

	// Test if "www." is within the url
	if (found!=std::string::npos){
		// Teste if "www." starts in the beginning of the url, or after "http://" or "https://"
		if ((!found)||(found >= 7 && found <= 8)){
			url = url.erase(found,found+delimitation.size());
		}
	}

	if (url.back() == '/'){
		url.pop_back();
	}

	// cout << "Normalized Url: " << this->url << " " << url << endl;

	return url;
}

string Url::getDomain(){
	CkSpider spider;
	CkString domain;

	spider.GetUrlDomain(this->url.c_str(), domain);

	return domain.getString();
}

bool Url::isBrDomain(){
	string domain = this->getDomain();
	string delimitation (".br");

	if (domain.back() == '/'){
		domain.pop_back();
	}

	size_t found = domain.find(delimitation);

	return (found == (domain.size()-delimitation.size()));

}
