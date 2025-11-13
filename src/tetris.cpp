#include "tetris.hpp"

bool power_of_two(uint32_t num) {
	if(num <= 0) return false;
	return ((num & (num-1)) == 0);
}

void skip(std::istream& is) {
    char c = 0;
    is >> c;
    is.putback(c);
}

void PARSE(std::istream& is, piece& p);
void QUADRANT(std::istream& is, piece& p, uint32_t i, uint32_t j, uint32_t side);
void PARENTHESES(std::ostream& os, const piece&  p, uint32_t i, uint32_t j, uint32_t side);

void PARSE(std::istream& is, piece& p) {

	skip(is);
	int side;
	is >> side;

	if(!is.good()){
		throw tetris_exception("PARSE(std::istream& is, piece& p): failed to read side or invalid format\n");
	}
	
	if(side <= 0){
		throw tetris_exception("PARSE(std::istream& is, piece& p): side <= 0\n");
	}

	if(!power_of_two(side)){
		throw tetris_exception("PARSE(std::istream& is, piece& p): side is not a power of two\n");
	}

	skip(is);
	int color;
	is >> color;

	if(!is.good()){
		throw tetris_exception("PARSE(std::istream& is, piece& p): failed to read color or invalid format\n");
	}

		
	if(color < 1 || color > 255){
		throw tetris_exception("PARSE(std::istream& is, piece& p) : color value < 1 || color value > 255\n");
	}

	piece tmp(static_cast<uint32_t>(side), static_cast<uint8_t>(color));
	

	QUADRANT(is, tmp, 0, 0, side);
	p = tmp;

}


void QUADRANT(std::istream& is, piece& p, uint32_t i, uint32_t j, uint32_t side) {

	skip(is);

	char c = 0;
	c = is.peek();

	if(c == '('){
		is >> c; 
		skip(is);
		c = is.peek();
		if(c == ')'){
			is >> c;
			for(uint32_t row = i; row < i+side; row++){
				for(uint32_t col = j; col < j+side; col++){
					p(row,col) = true;
				}
			}
		}else if(c == '(' || c == '['){

			if(side > 1){					
				uint32_t h_side = side/2;
				QUADRANT(is, p, i, j, h_side); 
				QUADRANT(is, p, i, j+h_side, h_side); 
				QUADRANT(is, p, i+h_side, j, h_side); 
				QUADRANT(is, p, i+h_side, j+h_side, h_side); 

				skip(is);
            	is >> c;
            	if(c != ')') {
                	throw tetris_exception("QUADRANT(std::istream& is, piece& p, uint32_t i, uint32_t j, uint32_t side): expected ')' after recursive quadrants\n");
            	}
            }else{
            	throw tetris_exception("QUADRANT(std::istream& is, piece& p, uint32_t i, uint32_t j, uint32_t side): quadrants can be furhter divided\n");
            }
		}else{
			throw tetris_exception("QUADRANT(std::istream& is, piece& p, uint32_t i, uint32_t j, uint32_t side): after '(' expected ')' otherwise '((' or '(['\n");
		}
	}else if(c == '['){
		is >> c; 
		skip(is);
		c = is.peek();
		if(c == ']'){
			is >> c;
		}else{
			throw tetris_exception("QUADRANT(std::istream& is, piece& p, uint32_t i, uint32_t j, uint32_t side): expected ']' after '['\n");
		}
	}else{
		throw tetris_exception("QUADRANT(std::istream& is, piece& p, uint32_t i, uint32_t j, uint32_t side): unexpected character. Permitted characters: '(', ')', '[', ']'\n");
	}

}


void PARENTHESES(std::ostream& os, const piece& p, uint32_t i, uint32_t j, uint32_t side) {
	if(p.empty(i,j,side)){
		os << "[]";
	}else if(p.full(i,j,side)){
		os << "()";
	}else{
		if(side > 1){
			uint32_t h_side = side/2;
			os << '(';
			PARENTHESES(os, p, i, j, h_side);
			PARENTHESES(os, p, i, j+h_side, h_side);
			PARENTHESES(os, p, i+h_side, j, h_side);
			PARENTHESES(os, p, i+h_side, j+h_side, h_side);
			os << ')';
		}
	}
}

piece::piece() {
	m_side = 0;
	m_color = 0;
	m_grid = nullptr;
}


piece::piece(uint32_t s, uint8_t c) {
	if(!power_of_two(s)) { 
		throw tetris_exception("piece::piece(uint32_t s, uint8_t c): side is not a power of two\n");
	}else if(s <= 0){
		throw tetris_exception("piece::piece(uint32_t s, uint8_t c): side <= 0");
	}else if(c == 0){
		throw tetris_exception("piece::piece(uint32_t s, uint8_t c) : color value == 0");
	}else{

		m_side = s;
		m_color = c;
		m_grid = new bool*[m_side];

		for(uint32_t i = 0; i < m_side; i++) {
			m_grid[i] = new bool[m_side];
		}


		for(uint32_t i = 0; i < m_side; i++) {
			for(uint32_t j = 0; j < m_side; j++) {
				m_grid[i][j] = false;
			}
		}
	}
}

piece::piece(piece const& rhs) {
	m_side = rhs.m_side;
	m_color = rhs.m_color;
	m_grid = new bool*[m_side];
	for(uint32_t i = 0; i < m_side; i++) {
		m_grid[i] = new bool[m_side];
	}

	for(uint32_t i = 0; i < m_side; i++) {
		for(uint32_t j = 0; j < m_side; j++) {
			m_grid[i][j] = rhs.m_grid[i][j];
		}
	}
}

piece::piece(piece&& rhs) : piece() {
	*this = std::move(rhs);
}

piece::~piece() {
	for(uint32_t i = 0; i < m_side; i++) {
		delete[] m_grid[i];
	}

	delete[] m_grid;
}


piece& piece::operator=(piece const& rhs) {
	if(this == &rhs) return *this;

	for(uint32_t i = 0; i < m_side; i++) {
		delete[] m_grid[i];
	}
	delete[] m_grid;

	m_side = rhs.m_side;
	m_color = rhs.m_color;
	m_grid = new bool*[rhs.m_side];

	for(uint32_t i = 0; i < m_side; i++) {
		m_grid[i] = new bool[m_side];
	}

	for(uint32_t i = 0; i < m_side; i++) {
		for(uint32_t j = 0; j < m_side; j++) {
			m_grid[i][j] = rhs.m_grid[i][j];
		}
	}

	return *this;
}

piece& piece::operator=(piece&& rhs) {
	if(this == &rhs) return *this;

	for(uint32_t i = 0; i < m_side; i++) {
		delete[] m_grid[i];
	}
	delete[] m_grid;

	m_side = rhs.m_side;
	m_color = rhs.m_color;
	m_grid = rhs.m_grid;

	rhs.m_side = 0;
	rhs.m_color = 0;
	rhs.m_grid = nullptr;

	return *this;
}


uint32_t piece::side() const {
	return m_side;
}

int piece::color() const {
	return m_color;
}

bool& piece::operator()(uint32_t i, uint32_t j) {
	if(i >= m_side || j >= m_side){
		throw tetris_exception("piece::operator()(uint32_t i, uint32_t j): (i,j) out of bound\n");
	}else{
		return m_grid[i][j];
	}
}

bool piece::operator()(uint32_t i, uint32_t j) const {
	if(i >= m_side || j >= m_side){
		throw tetris_exception("piece::operator()(uint32_t i, uint32_t j) const: (i,j) out of bound\n");
	}else{
		return m_grid[i][j];
	}
}

bool piece::empty() const {
	bool ret = true;
	for(uint32_t i = 0; i < m_side && ret; i++) {
		for(uint32_t j = 0; j < m_side && ret; j++){
			if(m_grid[i][j]) ret = false;
		}
	}
	return ret;
}

bool piece::full() const {
	bool ret = true;
	if(m_side <= 0){
		ret = false;
	}else{
		for(uint32_t i = 0; i < m_side && ret; i++) {
			for(uint32_t j = 0; j < m_side && ret; j++){
				if(!m_grid[i][j]) ret = false;
			}
		}
	}
	return ret;
}

bool piece::empty(uint32_t i, uint32_t j, uint32_t s) const {
	if(i+s > m_side || j+s > m_side){
		throw tetris_exception("piece::empty(uint32_t i, uint32_t j, uint32_t s) const: (i,j) out of bounds\n");
	}
	bool ret = true;
	for(uint32_t r = i; r < i+s && ret; r++){
		for(uint32_t c = j; c < j+s && ret; c++){
			if(m_grid[r][c])
				ret = false;
		}
	}
	return ret;
}

bool piece::full(uint32_t i, uint32_t j, uint32_t s) const {
	if(i+s > m_side || j+s > m_side){
		throw tetris_exception("piece::full(uint32_t i, uint32_t j, uint32_t s) const: (i,j) out of bounds\n");
	}
	bool ret = true;
	for(uint32_t r = i; r < i+s && ret; r++){
		for(uint32_t c = j; c < j+s && ret; c++){
			if(!m_grid[r][c])
				ret = false;
		}
	}
	return ret;
}

void piece::rotate() {
	bool** tmp = new bool*[m_side];
	for(uint32_t i = 0; i < m_side; i++){
		tmp[i] = new bool[m_side];
	}

	for(uint32_t i = 0; i < m_side; i++){
		for(uint32_t j = 0; j < m_side; j++){
			tmp[j][m_side-1-i] = m_grid[i][j];
		}
	}

	for(uint32_t i = 0; i < m_side; i++){
		for(uint32_t j = 0; j < m_side; j++){
			m_grid[i][j] = tmp[i][j];
		}
	}
	for(uint32_t i = 0; i < m_side; i++){
		delete[] tmp[i];
	}
	delete[] tmp;
}

void piece::cut_row(uint32_t i) {
	if(i >= m_side){
		throw tetris_exception("piece::cut_row(uint32_t i): try to remove an invalid row\n");
	}

	for(uint32_t r = i; r > 0; r--){
		for(uint32_t c = 0; c < m_side; c++){
			m_grid[r][c] = m_grid[r-1][c];
		}
	}

	for(uint32_t c = 0; c < m_side; c++){
		m_grid[0][c] = false;
	}
}

void piece::print_ascii_art(std::ostream& os) const {
	for(uint32_t i = 0; i < m_side+2; i++){
		os << '-';
	}
	os << '\n';
	for(uint32_t i = 0; i < m_side; i++) {
		os << '|';
		for(uint32_t j = 0; j < m_side; j++) {
			if(m_grid[i][j]){
				os << "\033[48;5;" << int(m_color) << "m" << ' ' << "\033[m";
			} else {
				os << ' ';
			}
		}

		os << '|' << '\n';
	}
	for(uint32_t i = 0; i < m_side+2; i++){
		os << '-';
	}
	os << '\n';
}

bool piece::operator==(piece const& rhs) const {
	if(m_side != rhs.m_side || m_color != rhs.m_color) return false;

	bool same = true;
	for(uint32_t i = 0; i < m_side && same; i++) {
		for(uint32_t j = 0; j < m_side; j++) {
			if(m_grid[i][j] != rhs.m_grid[i][j])
				same = false;
		}
	}
	return same;
}

bool piece::operator!=(piece const& rhs) const {
	return !(*this == rhs);
}

std::istream& operator>>(std::istream& is, piece& p) {
	PARSE(is, p);	
	return is;
}

std::ostream& operator<<(std::ostream& os, piece const& p) {
	uint32_t side = p.side();
	int color  = p.color();
	os << side << ' ' << color << ' ';
	PARENTHESES(os, p, 0, 0, side);
	return os;
}


tetris::tetris() {
	m_score = m_width = m_height = 0;
	m_field = nullptr;
}

tetris::tetris(uint32_t w, uint32_t h, uint32_t s) {
	if(w == 0 || h == 0){ 
		throw tetris_exception("tetris(uint32_t w, uint32_t h, uint32_t s): width and height have to be more than zero\n");
	}
	m_score = s;
	m_width = w;
	m_height = h;
	m_field = nullptr;
	
}

tetris::tetris(tetris const& rhs) {
	m_score = rhs.m_score;
	m_width = rhs.m_width;
	m_height = rhs.m_height;
	m_field = nullptr;

	for(const_iterator rhs_it = rhs.begin(); rhs_it != rhs.end(); rhs_it++){
		node* new_node = new node{{rhs_it->p, rhs_it->x, rhs_it->y}, nullptr};
		if(!m_field){
			m_field = new_node;
		}else{
			node* tmp = m_field;
			while(tmp->next)
				tmp = tmp->next;
			tmp->next = new_node;
		}
	}
}

tetris::tetris(tetris&& rhs) : tetris() {
	*this = std::move(rhs);
}

tetris::~tetris() {
	while(m_field){
		node* del = m_field;
		m_field = m_field->next;
		delete del;
	}
}


tetris& tetris::operator=(tetris const& rhs) {
	if(this != &rhs){
		while(m_field){
			node* del = m_field;
			m_field = m_field->next;
			delete del;
		}
		m_score = rhs.m_score;
		m_width = rhs.m_width;
		m_height = rhs.m_height;
		m_field = nullptr;

		for(const_iterator rhs_it = rhs.begin(); rhs_it != rhs.end(); ++rhs_it){
			node* new_node = new node{{rhs_it->p, rhs_it->x, rhs_it->y}, nullptr};
			if(!m_field){
				m_field = new_node;
			}else{
				node* tmp = m_field;
				while(tmp->next)
					tmp = tmp->next;
				tmp->next = new_node;
			}
		}
	}
	return *this;
}

tetris& tetris::operator=(tetris&& rhs) {
	if(this != &rhs){
		while(m_field){
			node* del = m_field;
			m_field = m_field->next;
			delete del;
		}
		m_score = rhs.m_score;
		m_width = rhs.m_width;
		m_height = rhs.m_height;
		m_field = rhs.m_field;

		rhs.m_score = 0;
		rhs.m_width = 0;
		rhs.m_height = 0;
		rhs.m_field = nullptr;
	}
	return *this;
}


bool tetris::operator==(tetris const& rhs) const { 
	bool ret = true;
	if(m_height != rhs.m_height || m_width != rhs.m_width || m_score != rhs.m_score)
		ret = false;

	auto it = begin();
	auto rhs_it = rhs.begin();
	for(; it != end() && rhs_it != rhs.end() && ret; ++it, ++rhs_it){
		if(it->p != rhs_it->p || it->x != rhs_it->x || it->y != rhs_it->y){			
			ret = false;
		}
	}

	if(it != end() || rhs_it != rhs.end())
		ret = false;

	return ret;
}

bool tetris::operator!=(tetris const& rhs) const {
	return !(*this == rhs);
}

int get_top_line(piece const& p){
	bool found_top = false;
	int top_filled_row = -1;
	for(uint32_t i = 0; i < p.side() && !found_top; i++){
		for(uint32_t j = 0; j < p.side() && !found_top; j++){
			if(p(i,j)){
				top_filled_row = i;
				found_top = true;
			}
		}
	}
	return top_filled_row;
}

void tetris::insert(piece const& p, int x) {
	int top_line = get_top_line(p);
	int start_y = 0;

	if(top_line == -1){
		add(p,x,start_y);
	}else{
		start_y = p.side()-1-top_line;
	

		if(!containment(p,x,start_y)){
			throw tetris_exception("GAME OVER\n");
		}
	

		int final_y = start_y;
		for(int y = start_y+1; containment(p,x,y); y++){
			final_y = y;
		}

		add(p,x,final_y);
	}

	bool cutted, shifted;
	do{
		cutted = false;
		shifted = false;
		// ricerca eventuale riga da tagliare
		for(int row = m_height-1; row >= 0; row--){
			uint32_t col = 0;
			for(auto it = begin(); it != end(); it++){
				uint32_t p_side = it->p.side();

				for(uint32_t i = 0; i < p_side; i++){
					for(uint32_t j = 0; j < p_side; j++){
						if(it->p(i,j)){
							int tetris_r = i+it->y-(p_side-1);
							if(tetris_r == row){
								col++;
							}
						}
					}
				}
			}
				
			if(col == m_width){
				cutted = true;
				m_score += m_width;

				for(auto it = begin(); it != end(); it++){
					uint32_t del_row = row + (it->p.side()-1) - it->y;
					if(del_row < it->p.side())
						it->p.cut_row(del_row);
				}
			}
		}

		// controllo se ci sono pezzi vuoti
		node* curr = m_field;
		node* prev = nullptr;
		while(curr){
			if(curr->tp.p.empty()){
				node* del = curr;

				if(curr == m_field && !prev){
					m_field = m_field->next;
					curr = m_field;
					delete del;
				}else{
					prev->next = curr->next;
					curr = curr->next;
					delete del;
				}
			}else{
				prev = curr;
				curr = curr->next;
			}
		}

		// gravità
		for(auto it = begin(); it != end(); it++){
			int old_y = it->y;
			while(containment(it->p, it->x, it->y+1)){
				it->y++;
			}

			if(it->y > old_y){
				shifted = true;
			}
		}
	}while(cutted || shifted);
}

void tetris::add(piece const& p, int x, int y) {
	if(!containment(p,x,y)){
		throw tetris_exception("add(piece const& p, int x, int y): The piece cannot be contained in the tetris field\n");
	}else{
		node* new_node = new node{{p,x,y}, nullptr};
		if(!m_field) m_field = new_node;
		else{
			new_node->next = m_field;
			m_field = new_node;
		}
	}
}

bool tetris::containment(piece const& p, int x, int y) const {
	bool contained = true;

	if(y < 0){
		contained = false;
	}else{
		// creo campo temporaneo
		bool** my_field = new bool*[m_height];
		for(uint32_t i = 0; i < m_height; i++){
			my_field[i] = new bool[m_width];
			for(uint32_t j = 0; j < m_width; j++){
				my_field[i][j] = false;
			}
		}

		// riempio il campo escludendo, se presente, il piece parametro di containment
		for(auto it = begin(); it != end(); it++){
			if(&p != &(it->p)){
				uint32_t p_side = it->p.side();
				for(uint32_t i = 0; i < p_side; i++){
					for(uint32_t j = 0; j < p_side; j++){
						if(it->p(i,j)){
						    int tetris_r = i + it->y - (p_side-1); 
						    int tetris_c = j + it->x;
						
						    if (tetris_r >= 0 && tetris_r < int(m_height) && tetris_c >= 0 && tetris_c < int(m_width)) {
						        my_field[tetris_r][tetris_c] = true;
						    }
						}
						
					}
				}
			}
		}

		for(uint32_t r = 0; r < p.side() && contained; r++){
			for(uint32_t c = 0; c < p.side() && contained; c++){
				if(p(r,c)){
					int tetris_r = static_cast<int>(r) + y - (static_cast<int>(p.side())-1);
					int tetris_c = static_cast<int>(c) + x;

					int i_m_height = static_cast<int>(m_height);
					int i_m_width = static_cast<int>(m_width);

					if(tetris_r < 0 || tetris_r >= i_m_height || tetris_c < 0 || tetris_c >= i_m_width || my_field[tetris_r][tetris_c]){
						contained = false;
					}
				}
			}
		}
		// elimino campo temporaneao
		for(uint32_t i = 0; i < m_height; i++){
			delete [] my_field[i];
		}
		delete [] my_field;
	}

	return contained;
}

void tetris::print_ascii_art(std::ostream& os) const {
	os << "SCORE: " << m_score << '\n';

	uint8_t my_field[m_height][m_width];
	for(uint32_t i = 0; i < m_height; i++){
		for(uint32_t j = 0; j < m_width; j++){
			my_field[i][j] = 0;
		}
	}

	for(auto it = begin(); it != end(); it++){
		uint32_t p_side = it->p.side();
		for(uint32_t i = 0; i < p_side; i++){
			for(uint32_t j = 0; j < p_side; j++){
				if(it->p(i,j) == true){
					uint32_t tetris_r = i+it->y-(p_side-1);
					uint32_t tetric_c = j+it->x;
					my_field[tetris_r][tetric_c] = it->p.color();
				}
			}
		}
	}

	for(uint32_t i = 0; i < m_width+2; i++){
		os << '-';
	}
	os << '\n';
	for(uint32_t i = 0; i < m_height; i++){
		os << '|';
		for(uint32_t j = 0; j < m_width; j++){
			if(my_field[i][j] != 0){
				os << "\033[48;5;" << int(my_field[i][j]) << "m" << ' ' << "\033[m";
			} else {
				os << ' ';
			}
		}
		os << '|';
		os << '\n';
	}

	for(uint32_t i = 0; i < m_width+2; i++){
		os << '-';
	}
	os << '\n';
}

tetris::iterator tetris::begin() {
	return iterator(m_field);
}

tetris::iterator tetris::end() {
	return iterator(nullptr);
}

tetris::const_iterator tetris::begin() const {
	return const_iterator(m_field);
}

tetris::const_iterator tetris::end() const {
	return const_iterator(nullptr);
}

uint32_t tetris::score() const {
	return m_score;
}

uint32_t tetris::width() const {
	return m_width;
}

uint32_t tetris::height() const {
	return m_height;
}

tetris::iterator::iterator(node* ptr){
	m_ptr = ptr;
}

tetris::iterator::reference tetris::iterator::operator*() {
	return m_ptr->tp;
}

tetris::iterator::pointer tetris::iterator::operator->() {
	return &(m_ptr->tp);
}

tetris::iterator& tetris::iterator::operator++() {
	m_ptr = m_ptr->next;
	return *this;
}

tetris::iterator tetris::iterator::operator++(int /*dummy*/) {
	iterator tmp = *this;
	m_ptr = m_ptr->next;
	return tmp;
}

bool tetris::iterator::operator==(iterator const& rhs) const {
	return m_ptr == rhs.m_ptr;
}

bool tetris::iterator::operator!=(iterator const& rhs) const {
	return !(*this == rhs);
}

tetris::const_iterator::const_iterator(node const* ptr) {
	m_ptr = ptr;
}

tetris::const_iterator::reference tetris::const_iterator::operator*() const {
	return m_ptr->tp;
}

tetris::const_iterator::pointer tetris::const_iterator::operator->() const {
	return &(m_ptr->tp);
}

tetris::const_iterator& tetris::const_iterator::operator++() {
	m_ptr = m_ptr->next;
	return *this;
}

tetris::const_iterator tetris::const_iterator::operator++(int /*dummy*/) {
	const_iterator tmp = *this;
	m_ptr = m_ptr->next;
	return tmp;
}

bool tetris::const_iterator::operator==(const_iterator const& rhs) const {
	return m_ptr == rhs.m_ptr;
}

bool tetris::const_iterator::operator!=(const_iterator const& rhs) const {
	return !(*this == rhs);
}

std::istream& operator>>(std::istream& is, tetris& t) {
	uint32_t score;
	uint32_t width;
	uint32_t height;

	skip(is);
	is >> score;
	skip(is);
	is >> width;
	skip(is);
	is >> height;

	tetris tmp(width, height, score);
	t = tmp;
	
	
	skip(is);
	piece p;
	int x,y;

	while(is){
		skip(is);
		is >> p;
		skip(is);
		is >> x;
		skip(is);
		is >> y;

		t.add(p,x,y);
		
		skip(is);
	}

	return is;
}

std::ostream& operator<<(std::ostream& os, tetris const& t) {
	os << t.score() << ' ' << t.width() << ' ' << t.height() << '\n';
	
	for(auto it = t.begin(); it != t.end(); it++){
		os << it->p << ' ' << it->x << ' ' << it->y << '\n'; 
	}

	return os;
}