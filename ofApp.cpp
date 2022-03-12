#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofSetLineWidth(1.5);

	for (int i = 0; i < 2; i++) {

		this->noise_seed_list.push_back(glm::vec2(ofRandom(1000), ofRandom(1000)));
	}

	this->font_size = 30;
	ofTrueTypeFontSettings font_settings("fonts/msgothic.ttc", this->font_size);
	font_settings.antialiased = true;
	font_settings.addRanges(ofAlphabet::Japanese);
	this->font.load(font_settings);

	this->words = {

		//u8"!", u8"#", u8"$", u8"%", u8"&",
		//u8"'", u8"(", u8")", u8"*", u8"+",
		//u8",", u8"-", u8".", u8"/", u8"0",
		//u8"1", u8"2", u8"3", u8"4", u8"5",
		//u8"6", u8"7", u8"8", u8"9", u8"0",
		//u8":", u8";", u8"<", u8"=", u8">",
		//u8"?", u8"@",

		//u8"A", u8"B", u8"C", u8"D", u8"E",
		//u8"F", u8"G", u8"H", u8"I", u8"J",
		//u8"K", u8"L", u8"M", u8"N", u8"O",
		//u8"P", u8"Q", u8"R", u8"S", u8"T",
		//u8"U", u8"V", u8"W", u8"X", u8"Y", u8"Z",

		//u8"a", u8"b", u8"c", u8"d", u8"e",
		//u8"f", u8"g", u8"h", u8"i", u8"j",
		//u8"k", u8"l", u8"m", u8"n", u8"o",
		//u8"p", u8"q", u8"r", u8"s", u8"t",
		//u8"u", u8"v", u8"w", u8"x", u8"y", u8"z",

		u8"�", u8"�", u8"�", u8"�", u8"�",
		u8"�", u8"�", u8"�", u8"�", u8"�",
		u8"�", u8"�", u8"�", u8"�", u8"�",
		u8"�", u8"�", u8"�", u8"�", u8"�",
		u8"�", u8"�", u8"�", u8"�", u8"�",
		u8"�", u8"�", u8"�", u8"�", u8"�",
		u8"�", u8"�", u8"�", u8"�", u8"�",
		u8"�", u8"�", u8"�",
		u8"�", u8"�", u8"�", u8"�", u8"�",
		u8"�", u8"�", u8"�",
	};

	// �z�F�f�U�C�� �\�t�g�O���[�� P114
	this->color_palette.push_back(ofColor(97, 157, 110));
	this->color_palette.push_back(ofColor(119, 180, 106));
	this->color_palette.push_back(ofColor(143, 173, 91));
	this->color_palette.push_back(ofColor(145, 195, 161));
	this->color_palette.push_back(ofColor(93, 169, 145));
	this->color_palette.push_back(ofColor(249, 229, 128));
	this->color_palette.push_back(ofColor(131, 193, 215));
	this->color_palette.push_back(ofColor(255, 255, 255));
}

//--------------------------------------------------------------
void ofApp::update() {

	int index = 0;
	auto radius = ofMap(ofNoise(39,ofGetFrameNum() * 0.01), 0, 1, 100, 350);
	for (auto& noise_seed : this->noise_seed_list) {

		auto deg = ofGetFrameNum() * 3 + index * 180;
		auto next_deg = deg + 3;
	
		auto location = glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD));
		auto next = glm::vec2(radius * cos(next_deg * DEG_TO_RAD), radius * sin(next_deg * DEG_TO_RAD));

		auto distance = location - next;
		distance *= 1;

		for (int i = 0; i < 2; i++) {

			auto future = location + distance * 8;
			auto random_deg = ofRandom(360);
			future += glm::vec2(30 * cos(random_deg * DEG_TO_RAD), 30 * sin(random_deg * DEG_TO_RAD));
			auto future_distance = future - location;

			this->location_list.push_back(location);
			this->velocity_list.push_back(glm::normalize(future_distance) * glm::length(distance));
			this->color_list.push_back(this->color_palette[ofRandom(1, this->color_palette.size())]);
			this->word_index_list.push_back(ofRandom(this->words.size()));
		}

		index++;
	}

	for (int i = this->location_list.size() - 1; i > -1; i--) {

		this->location_list[i] += this->velocity_list[i];
		this->velocity_list[i] *= 1.01;

		if (glm::distance(glm::vec2(), this->location_list[i]) > 720) {

			this->location_list.erase(this->location_list.begin() + i);
			this->velocity_list.erase(this->velocity_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
			this->word_index_list.erase(this->word_index_list.begin() + i);
		}
	}

}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWindowSize() * 0.5);

	int index = 0;
	for (auto& location : this->location_list) {

		ofPushMatrix();
		ofTranslate(location);

		ofSetColor(this->color_list[index]);

		auto angle = std::atan2(location.y, location.x);
		ofRotate(angle * RAD_TO_DEG);

		this->font.drawString(this->words[this->word_index_list[index]], 0, 0);
	
		ofPopMatrix();


		index++;
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}