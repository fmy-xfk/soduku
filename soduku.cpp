#include<bits/stdc++.h>
using namespace std;

#define INVALID -1

//A public function for splitting a string 
vector<string> split_str(const string& str, const string& delimiter) {
	vector<string> result;
	string::size_type pos1, pos2;
	pos2=str.find(delimiter);
	pos1=0;
	while (string::npos!=pos2) {
		result.push_back(str.substr(pos1, pos2-pos1));
		pos1=pos2+delimiter.size();
		pos2=str.find(delimiter,pos1);
	}
	if (pos1!=str.length()) {
		result.push_back(str.substr(pos1));
	}
	return result;
}

//A struct used to describe the coordinate of a tile
struct pt {
	int x,y;
};

//A struct used to descirbe a rule that the sum of certain tiles is a given number
struct sum_rule {
	vector<pt> pos;
	int sum;
	//A function used to initialize the struct. The parameter string should be given like "A1+B2=10".
	sum_rule(string &s) {
		auto ret0=split_str(s,"=");
		assert(ret0.size()==2);
		sum=atoi(ret0[1].c_str());
		auto ret=split_str(ret0[0],"+");
		for(auto v:ret) {
			assert(v[0]>='A' && v[0]<='I');
			assert(v[1]>='1' && v[1]<='9');
			pos.push_back({v[0]-'A',v[1]-'1'});
		}
	}
};

//Main class used to solve a soduku quiz.
class soduku {
		int map[9][9]; //Indicate the soduku game map.
		int bel[9][9]; //Indicate which rule a tile is subject to.
		vector<pt> tiles; //Store the coordinate of initial blank tiles. 
		int tiles_ptr; //Indicate the index of current tile.
		int tiles_cnt; //Indicate the count of initial blank tiles.
		vector<sum_rule> rules; //Store the rules of summation.
		
		//Get the possible numbers to be fill in the given tile.
		vector<int> _get_possible(int i,int j) {
			bool have[10];
			memset(have,0,sizeof(have));
			for(int k=0; k<9; ++k) {
				have[this->map[i][k]]=true;
				have[this->map[k][j]]=true;
			}
			int iend=3*(i/3+1),jend=3*(j/3+1);
			for(int ki=i/3*3; ki<iend; ++ki) {
				for(int kj=j/3*3; kj<jend; ++kj) {
					have[this->map[ki][kj]]=true;
				}
			}
			int b=this->bel[i][j]-1,maxv=9,minv=1,bcnt=-1,msum=0;
			if(b>=0) {
				bcnt=0;
				for(auto p:rules[b].pos) {
					if(this->map[p.x][p.y]==0)++bcnt;
					else msum+=this->map[p.x][p.y];
				}
				maxv=min(9,rules[b].sum-msum);
				minv=max(1,rules[b].sum-msum-(bcnt-1)*(9+9-bcnt+2)/2);
			}
			vector<int> ret;
			if(bcnt==1) {
				if(!have[maxv] && maxv>0 && maxv<=9) ret.push_back(maxv);
			} else {
				for(int k=minv; k<=maxv; ++k) {
					if(!have[k]) ret.push_back(k);
				}
			}
			return ret;
		}
		
		//Get the next tile to be determined.
		pt _get_next_tile() {
			return tiles[tiles_ptr++];
		}
		
		//A recursive function used to try to fill a given tile.
		bool _tryfill(int i,int j) {
			auto aval=this->_get_possible(i,j);
			int len=aval.size();
			if(len==0) return false;
			this->map[i][j]=aval[0];
			auto nxt_pos=this->_get_next_tile();
			if(nxt_pos.x==INVALID && nxt_pos.y==INVALID) return true;
			for(int k=0; k<len; ++k) {
				this->map[i][j]=aval[k];
				if(this->_tryfill(nxt_pos.x,nxt_pos.y)) return true;
			}
			this->map[i][j]=0;
			--tiles_ptr;
			return false;
		}
	public:
		/*
		* Initialization. 
		* mp: should be a vector containing 9 string, each has 9 characters ranging from 1 to 9. 
			Note that blank tiles should be represent by '_'.
		* mrules: should be a vector of the raw string of rules.
		*/
		soduku(vector<string> &mp,vector<string> &mrules) {
			tiles_cnt=tiles_ptr=0;
			memset(this->bel,0,sizeof(this->bel));
			if(mp.size()==0) {
				for(int i=0; i<9; ++i) {
					for(int j=0; j<9; ++j) {
						this->map[i][j]=0;
						tiles.push_back({i,j});
						++tiles_cnt;
					}
				}
			} else {
				for(int i=0; i<9; ++i) {
					for(int j=0; j<9; ++j) {
						if(mp[i][j]=='_') {
							this->map[i][j]=0;
							tiles.push_back({i,j});
							++tiles_cnt;
						} else {
							this->map[i][j]=mp[i][j]-'0';
						}
					}
				}
			}
			tiles.push_back({INVALID,INVALID});
			int n=mrules.size();
			for(int i=1; i<=n; ++i) {
				sum_rule r(mrules[i-1]);
				rules.push_back(r);
				for(auto p:r.pos) {
					this->bel[p.x][p.y]=i;
				}
			}
		}
		
		//Function to solve the quiz.
		bool solve() {
			auto nxt_pos=this->_get_next_tile();
			if(nxt_pos.x==INVALID && nxt_pos.y==INVALID) return true;
			return this->_tryfill(nxt_pos.x,nxt_pos.y);
		}
		
		//Function to print the result.
		void print() {
			for(int i=0; i<9; ++i) {
				for(int j=0; j<9; ++j) {
					if(map[i][j]!=0) {
						cout<<map[i][j];
					} else {
						cout<<'_';
					}
				}
				cout<<endl;
			}
		}
};

int main() {
	vector<string> mps,rules;
	
	//Read the initial map from the keyboard.
	string s;
	for(int i=0; i<9; ++i) {
		cin>>s;
		mps.push_back(s);
	}
	
	//Read additional rules of summation.
	int n;
	cin>>n;
	for(int i=0; i<n; ++i) {
		cin>>s;
		rules.push_back(s);
	}
	
	//Create a soduku quize instance.
	soduku mt(mps,rules);
	
	//Calculate time and solve the quiz.
	auto start=chrono::high_resolution_clock::now();
	auto ret=mt.solve();
	auto end=chrono::high_resolution_clock::now();
	chrono::duration<double,milli>elapsedTime=end-start;
	
	//Print result.
	cout<<"Duration:"<<elapsedTime.count()/1000<<"s"<<endl;
	if(ret) {
		cout<<"Yes"<<endl;
		mt.print();
	} else {
		cout<<"No"<<endl;
	}
	return 0;
}

