#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

int main() {
	fstream infile("..\\acs\\DnD_Settings.h", ios::in);
	if(infile.is_open()) {
		fstream outfile("dnd_cvars.txt", ios::out);
		cout << "Found file. Processing...\n";
		
		vector<string> to_print;
		string line, tmp;
		stringstream ss;
		size_t index = 0;
		bool replace_maxlives = false;
		bool serverflag_read = false, servervalue_read = false, dndflag_read = false, dndvalue_read = false;
		bool serverflag_done = false, servervalue_done = false, dndflag_done = false, dndvalue_done = false;
		
		while(getline(infile, line)) {
			if(!serverflag_done) {
				if(!serverflag_read && line.find("FlagCheckList") != string::npos)
					serverflag_read = true;
				else if(serverflag_read) {
					if(line[0] == '}') {
						serverflag_done = true;
						continue;
					}
					// add them to vector
					if(*(line.end() - 1) == ',')
						tmp = line.substr(2, line.length() - 4);
					else
						tmp = line.substr(2, line.length() - 3);
					to_print.push_back(tmp);
				}
			}
			
			if(!servervalue_done) {
				if(!servervalue_read && line.find("FlagAcceptedValues") != string::npos)
					servervalue_read = true;
				else if(servervalue_read) {
					if(line[0] == '}') {
						servervalue_done = true;
						continue;
					}
					
					if(!replace_maxlives) {
						// hardcoded as 1 life in hardcore
						ss << to_print[index] << " 1";
						replace_maxlives = true;
					}
					else {
						if(*(line.end() - 1) == ',')
							tmp = line.substr(1, line.length() - 2);
						else
							tmp = line.substr(1, line.length() - 1);
						
						ss << to_print[index] << ' ' << tmp;
					}

					to_print[index] = ss.str();
					ss.str("");
					//cout << to_print[index] << '\n';
					++index;
				}
			}
			
			// remember this index, we'll continue from it
			if(!dndflag_done) {
				if(!dndflag_read && line.find("DNDFlagCheckList") != string::npos)
					dndflag_read = true;
				else if(dndflag_read) {
					if(line[0] == '}') {
						dndflag_done = true;
						continue;
					}
					
					// newlines
					if(line.length() < 2)
						continue;
					// add them to vector
					if(*(line.end() - 1) == ',')
						tmp = line.substr(2, line.length() - 4);
					else
						tmp = line.substr(2, line.length() - 3);
					to_print.push_back(tmp);
				}
			}
			
			if(!dndvalue_done) {
				if(!dndvalue_read && line.find("DNDFlagAcceptedValues") != string::npos)
					dndvalue_read = true;
				else if(dndvalue_read) {
					if(line[0] == '}') {
						dndvalue_done = true;
						break;
					}
					
					// newlines
					if(line.length() < 2)
						continue;
					
					if(*(line.end() - 1) == ',')
						tmp = line.substr(1, line.length() - 2);
					else
						tmp = line.substr(1, line.length() - 1);
					
					ss << to_print[index] << ' ' << tmp;

					to_print[index] = ss.str();
					ss.str("");
					//cout << to_print[index] << '\n';
					++index;
				}
			}
		}
		for(auto s : to_print)
			outfile << s << '\n';
		infile.close();
		outfile.close();
		cout << "\nDone. See \"dnd_cvars.txt\" for the required cvars.";
	}
	else
		cout << "File not found.\n";
	return 0;
}
