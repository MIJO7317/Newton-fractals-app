#pragma once

#include "ofMain.h"
#include "ofxPostProcessing.h"
#include "ofxSvg.h"
#include "fparser.hh"
#include "FractalsGenerator.h"
#include <vector>

class ofApp : public ofBaseApp{

	public:
		enum class StringPosition
		{
			CENTER,
			LEFT,
			RIGHT
		};

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void mouseScrolled(int x, int y, float scrollX, float scrollY);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void DrawFunctionInput(int x, int y, int width, int height);
		void DrawArgumentsInput(int x, int y, int width, int height);
		void DrawRoots(int x, int y, int width, int height);
		void DrawSingularities(int x, int y, int width, int height);
		void DrawTitle(int x, int y, int width, int height, string title);
		void DrawSubtitle(int x, int y, int width, int height, string subtitle);
		void DrawNotification(int x, int y, string notification);
		void DrawStringInRect(int x, int y, int width, int height, string s, ofTrueTypeFont font, StringPosition pos = StringPosition::CENTER);
		void DrawFrame(int x, int y, int width, int height, int border);
		void DrawLegend(int x, int y, int width, int height, string name_legend);
		void DrawRoot(int x, int y, int d);
		void DrawSingularity(int x, int y, int d);
		float StringWidth(string s, ofTrueTypeFont font);

		ofxPostProcessing post;
		ofTrueTypeFont function_font;
		ofTrueTypeFont symbols_font;
		ofTrueTypeFont title_font;
		ofTrueTypeFont subtitle_font;
		ofTrueTypeFont notification_font;
		ofTrueTypeFont dynamic_font;
		ofxSVG cursor;
		ofImage cursor_png;
		ofColor text_color = ofColor::gray;
		ofColor form_color = ofColor::white;
		ofColor wait_color_function = ofColor::lightGray;
		ofColor wait_color_eps = ofColor::lightGray;
		ofColor wait_color_a = ofColor::lightGray;
		ofColor wait_color_quality = ofColor::lightGray;
		int ofset = 10;
		ofRectangle function_rect;
		ofRectangle eps_rect;
		ofRectangle a_rect;
		ofRectangle quality_rect;
		ofRectangle roots_rect;
		ofRectangle singularities_rect;
		ofRectangle button_rect;
		bool input_function = false;
		bool input_eps = false;
		bool input_a = false;
		bool input_quality = false;
		string function = "";
		string eps = "";
		string a = "";
		string quality = "";
		int time;
		bool trig = true;
		int scroll_roots = 0;
		int scroll_singularities = 0;
		int move_function = 0;
		size_t function_begin_element = 0;
		int move_eps = 0;
		size_t eps_begin_element = 0;
		int move_a = 0;
		size_t a_begin_element = 0;
		int move_quality = 0;
		size_t quality_begin_element = 0;
		std::vector<string> roots;// = { "100000000+2000000i", "1+3i", "-1+10i", "0.1-0i", "111-7i", "12-8i" };
		std::vector<string> singularities;// = { "0.001+0.001i", "1000+300i", "-1e6+10e5i", "10-0.001i", "111-70i", "12.156-80.123i", "1+2i", "1+3i", "-1+10i", "0.1-0i", "111-7i", "12-8i" };
		std::vector<std::pair<int, int>> draw_roots;
		std::vector<std::pair<int, int>> draw_singularities;
		FunctionParser_cd fparser;
		double num_eps;
		double num_a;
		double num_quality;
		bool ia_eps = false;
		bool ia_a = false;
		bool ia_quality = false;
		string err_message_eps;
		string err_message_a;
		string err_message_quality;
		bool button_active = false;
		FractalGenerator generator;
		ofImage fractal;
		bool loading = false;
};

