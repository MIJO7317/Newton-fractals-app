#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	this->post.init(ofGetWidth(), ofGetHeight());
	this->post.createPass<FxaaPass>();
	this->function_font.load("fonts/FreeSerifItalic.otf", 24);
	this->title_font.load("fonts/FreeSerif.ttf", 32);
	this->subtitle_font.load("fonts/FreeSerif.ttf", 24);
	this->notification_font.load("fonts/FreeSerif.ttf", 16);
	this->symbols_font.load("fonts/Greek_Italic.ttf", 24);
	this->dynamic_font.load("fonts/FreeSerifItalic.otf", 30);
	this->time = ofGetElapsedTimeMillis();
	fparser.AddConstant("pi", 4. * atan(1.));
	fparser.AddConstant("e", exp(1.));
}

//--------------------------------------------------------------
void ofApp::update()
{
	if (loading == true && !generator.isThreadRunning()) 
	{
		this->roots.clear();
		this->singularities.clear();
		this->draw_roots.clear();
		this->draw_singularities.clear();
		fractal.getPixelsRef() = generator.image;
		fractal.update();
		for (auto& root : generator.roots)
		{
			std::stringstream ss1;
			ss1 << std::fixed << std::setprecision(log10(1. / generator.eps)) << root.real();
			string root_x = ss1.str();
			std::stringstream ss2;
			ss2 << std::fixed << std::setprecision(log10(1. / generator.eps)) << root.imag();
			string root_y = ss2.str();
			this->roots.push_back(root_x + (root.imag() > 0 ? "+" : "") + root_y + "i");
			if (std::abs(root.imag()) < generator.a && std::abs(root.real()) < generator.a)
			{
				this->draw_roots.emplace_back(2. * 30. + 467. + 50 + (467. - 50) / 2. + (467. -50)/ (2. * generator.a) * root.real(), 30. + (467. -50)/ 2. - (467. - 50) / (2. * generator.a) * root.imag());
			}
		}
		for (auto& singularity : generator.singularities)
		{
			std::stringstream ss1;
			ss1 << std::fixed << std::setprecision(log10(1. / generator.eps)) << singularity.real();
			string singularity_x = ss1.str();
			std::stringstream ss2;
			ss2 << std::fixed << std::setprecision(log10(1. / generator.eps)) << singularity.imag();
			string singularity_y = ss2.str();
			this->singularities.push_back(singularity_x + (singularity.imag() > 0 ? "+" : "") + singularity_y + "i");
			if (std::abs(singularity.imag()) < generator.a && std::abs(singularity.real()) < generator.a)
			{
				this->draw_singularities.emplace_back(2. * 30. + 467. + 50 + (467. - 50) / 2. + (467. - 50) / (2. * generator.a) * singularity.real(), 30. + (467. - 50) / 2. - (467. - 50) / (2. * generator.a) * singularity.imag());
			}
		}
		loading = false;
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofFill();
	ofBackground(ofColor::white);
	this->post.begin();
	if (fractal.isAllocated())
		fractal.draw(2 * 30 + 467 + 50, 30, 467 - 50, 467 - 50);
	this->post.end();
	ofSetColor(ofColor::white);
	for (auto& root : draw_roots)
		this->DrawRoot(root.first, root.second, 12.5);
	for (auto& singularity : draw_singularities)
		this->DrawSingularity(singularity.first, singularity.second, 12.5);
	this->DrawTitle(30, 30, 467, 50, "Input");
	this->DrawFunctionInput(30, 30 + 50 + this->ofset, 467, 100);
	this->DrawArgumentsInput(30, 30 + 50 + 100 + 2*this->ofset, 467, 100);
	ofSetColor(ofColor::white);
	this->DrawTitle(30, 30 + 50 + 2 * 100 + 3 * this->ofset, 467, 50, "Output");
	if(this->function_rect.inside(ofRectangle(ofGetMouseX(), ofGetMouseY(), 1, 1)) || this->input_function || this->function.size() > 0)
		this->wait_color_function = ofColor::white;
	else
		this->wait_color_function = ofColor::lightGray;
	if (this->eps_rect.inside(ofRectangle(ofGetMouseX(), ofGetMouseY(), 1, 1)) || this->input_eps || this->eps.size() > 0)
		this->wait_color_eps = ofColor::white;
	else
		this->wait_color_eps = ofColor::lightGray;
	if (this->a_rect.inside(ofRectangle(ofGetMouseX(), ofGetMouseY(), 1, 1)) || this->input_a || this->a.size() > 0)
		this->wait_color_a = ofColor::white;
	else
		this->wait_color_a = ofColor::lightGray;
	if (this->quality_rect.inside(ofRectangle(ofGetMouseX(), ofGetMouseY(), 1, 1)) || this->input_quality || this->quality.size() > 0)
		this->wait_color_quality = ofColor::white;
	else
		this->wait_color_quality = ofColor::lightGray;

	if (std::fabs(ofGetElapsedTimeMillis() - this->time) > 500)
	{
		this->trig = !this->trig;
		this->time = ofGetElapsedTimeMillis();
	}
	if (this->trig)
	{
		ofSetColor(this->text_color);
		ofSetLineWidth(2);
		if (this->input_function)
		{
			string new_function = this->function.substr(this->function_begin_element, this->function.size() + this->move_function - this->function_begin_element);
			ofDrawLine(this->function_rect.x + this->StringWidth(new_function, this->function_font), this->function_rect.y, this->function_rect.x + this->StringWidth(new_function, this->function_font), this->function_rect.y + this->function_rect.height);
		}
		if (this->input_eps)
		{
			string new_eps = this->eps.substr(this->eps_begin_element, this->eps.size() + this->move_eps - this->eps_begin_element);
			ofDrawLine(this->eps_rect.x + this->StringWidth(new_eps, this->function_font), this->eps_rect.y, this->eps_rect.x + this->StringWidth(new_eps, this->function_font), this->eps_rect.y + this->eps_rect.height);
		}
		if (this->input_a)
		{
			string new_a = this->a.substr(this->a_begin_element, this->a.size() + this->move_a - this->a_begin_element);
			ofDrawLine(this->a_rect.x + this->StringWidth(new_a, this->function_font), this->a_rect.y, this->a_rect.x + this->StringWidth(new_a, this->function_font), this->a_rect.y + this->a_rect.height);
		}
		if (this->input_quality)
		{
			string new_quality = this->quality.substr(this->quality_begin_element, this->quality.size() + this->move_quality - this->quality_begin_element);
			ofDrawLine(this->quality_rect.x + this->StringWidth(new_quality, this->function_font), this->quality_rect.y, this->quality_rect.x + this->StringWidth(new_quality, this->function_font), this->quality_rect.y + this->quality_rect.height);
		}
	}

	ofSetColor(this->text_color);
	float displacement = 0;
	for (auto symbol : this->function)
	{
		int symbol_x = displacement - this->StringWidth(this->function.substr(0, this->function_begin_element) , this->function_font);
		ofSetColor(this->text_color.r, this->text_color.g, this->text_color.b, symbol_x < 0 || symbol_x > this->function_rect.width ? 0 : 255);
		this->function_font.drawString(string(1, symbol), this->function_rect.x + symbol_x, this->function_rect.y + this->function_rect.height / 2. + this->function_font.stringHeight("(") / 2 - 0.3 * this->function_font.getSize());
		displacement += this->function_font.stringWidth(string(1, symbol));
	}
	displacement = 0;
	for (auto symbol : this->eps)
	{
		int symbol_x = displacement - this->StringWidth(this->eps.substr(0, this->eps_begin_element), this->function_font);
		ofSetColor(this->text_color.r, this->text_color.g, this->text_color.b, symbol_x < 0 || symbol_x > this->eps_rect.width ? 0 : 255);
		this->function_font.drawString(string(1, symbol), this->eps_rect.x + symbol_x, this->eps_rect.y + this->eps_rect.height / 2. + this->function_font.stringHeight("(") / 2 - 0.3 * this->function_font.getSize());
		displacement += this->function_font.stringWidth(string(1, symbol));
	}
	displacement = 0;
	for (auto symbol : this->a)
	{
		int symbol_x = displacement - this->StringWidth(this->a.substr(0, this->a_begin_element), this->function_font);
		ofSetColor(this->text_color.r, this->text_color.g, this->text_color.b, symbol_x < 0 || symbol_x > this->a_rect.width ? 0 : 255);
		this->function_font.drawString(string(1, symbol), this->a_rect.x + symbol_x, this->a_rect.y + this->a_rect.height / 2. + this->function_font.stringHeight("(") / 2 - 0.3 * this->function_font.getSize());
		displacement += this->function_font.stringWidth(string(1, symbol));
	}
	displacement = 0;
	for (auto symbol : this->quality)
	{
		int symbol_x = displacement - this->StringWidth(this->quality.substr(0, this->quality_begin_element), this->function_font);
		ofSetColor(this->text_color.r, this->text_color.g, this->text_color.b, symbol_x < 0 || symbol_x > this->quality_rect.width ? 0 : 255);
		this->function_font.drawString(string(1, symbol), this->quality_rect.x + symbol_x, this->quality_rect.y + this->quality_rect.height / 2. + this->function_font.stringHeight("(") / 2 - 0.3 * this->function_font.getSize());
		displacement += this->function_font.stringWidth(string(1, symbol));
	}

	this->DrawRoots(30, 30 + 3 * 50 + 2 * 100 + 3 * this->ofset - this->function_font.stringHeight("1"), 233, 268 + this->function_font.stringHeight("1"));
	this->DrawSingularities(30 + 233, 30 + 3 * 50 + 2 * 100 + 3 * this->ofset - this->function_font.stringHeight("1"), 233, 268 + this->function_font.stringHeight("1"));
	this->DrawSubtitle(30, 30 + 2 * 50 + 2 * 100 + 3 * this->ofset, 467 / 2., 50, "Roots");
	this->DrawSubtitle(30 + 467 / 2., 30 + 2 * 50 + 2 * 100 + 3 * this->ofset, 467 / 2., 50, "Singularities");
	ofSetColor(this->text_color);
	ofSetLineWidth(2);
	ofDrawLine(30 + 467 / 2., 30 + 2 * 50 + 2 * 100 + 3 * this->ofset, 30 + 467 / 2., 30 + 2 * 50 + 2 * 100 + 3 * this->ofset + 50 + 268);

	this->button_rect = ofRectangle(30, 30 + 3 * 50 + 2 * 100 + 4 * this->ofset + 268, 467, 50);
	ofNoFill();
	ofSetColor(this->text_color);
	ofSetLineWidth(2);
	ofDrawRectangle(30, 30 + 3 * 50 + 2 * 100 + 4 * this->ofset + 268, 467, 50);
	ofSetColor(this->text_color);
	ofSetLineWidth(2);
	this->subtitle_font.drawString("Generate", 30 + 467 / 2. - this->subtitle_font.stringWidth("Generate") / 2, 30 + 3 * 50 + 2 * 100 + 4 * this->ofset + 268 + 50 / 2. + this->subtitle_font.stringHeight("Generate") / 2);
	ofFill();
	if (this->button_rect.inside(ofRectangle(ofGetMouseX(), ofGetMouseY(), 1, 1)) && !this->loading)
	{
		this->ia_a = this->ia_eps = this->ia_quality = false;
		this->err_message_eps = this->err_message_a = this->err_message_quality = "";
		try
		{
			this->num_eps = std::stod(this->eps);
		}
		catch (const std::invalid_argument& ia)
		{
			this->err_message_eps = "Invalid \"epsilon\" argument";
			this->ia_eps = true;
		}
		try
		{
			this->num_a = std::stod(this->a);
		}
		catch (const std::invalid_argument& ia)
		{
			this->err_message_a = "Invalid \"a\" argument";
			this->ia_a = true;
		}
		try
		{
			this->num_quality = std::stod(this->quality);
		}
		catch (const std::invalid_argument& ia)
		{
			this->err_message_quality = "Invalid \"quality\" argument";
			this->ia_quality = true;
		}
		if (this->function.empty() || this->eps.empty() || this->a.empty() || this->quality.empty())
		{
			this->DrawNotification(ofGetMouseX(), ofGetMouseY(), "Fill in ALL the input fields");
			this->button_active = false;
		}
		else if (fparser.Parse(function, "z") >= 0)
		{
			this->DrawNotification(ofGetMouseX(), ofGetMouseY(), string(fparser.ErrorMsg()));
			this->button_active = false;
		}
		else if (this->ia_eps)
		{
			this->DrawNotification(ofGetMouseX(), ofGetMouseY(), this->err_message_eps);
			this->button_active = false;
		}
		else if (this->ia_a)
		{
			this->DrawNotification(ofGetMouseX(), ofGetMouseY(), this->err_message_a);
			this->button_active = false;
		}
		else if (this->ia_quality)
		{
			this->DrawNotification(ofGetMouseX(), ofGetMouseY(), this->err_message_quality);
			this->button_active = false;
		}
		else
		{
			ofSetColor(ofColor::lightGray);
			ofDrawRectangle(30 + 1, 30 + 3 * 50 + 2 * 100 + 4 * this->ofset + 268 + 1, 467 - 2, 50 - 2);
			ofSetColor(this->text_color);
			ofSetLineWidth(2);
			this->subtitle_font.drawString("Generate", 30 + 467 / 2. - this->subtitle_font.stringWidth("Generate") / 2, 30 + 3 * 50 + 2 * 100 + 4 * this->ofset + 268 + 50 / 2. + this->subtitle_font.stringHeight("Generate") / 2);
			this->button_active = true;
		}
	}
	this->DrawFrame(2 * 30 + 467, 30, 467, 467, 50);
	this->DrawLegend(2 * 30 + 467 + 50, 30 + this->ofset + 467, 467 - 50, 100, "Legend");
	if (this->loading)
	{
		ofNoFill();
		ofSetColor(this->text_color);
		ofDrawRectangle(2 * 30 + 467 + 50, ofGetHeight() - 30 - 50, 467 - 50, 50);
		ofFill();
		ofSetColor(ofColor::lightGray);
		ofDrawRectangle(2 * 30 + 467 + 50 + 1, ofGetHeight() - 30 - 50 + 1, (467 - 50 - 2) * this->generator.wait, 50-2);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (this->input_function)
	{
		this->trig = true;
		if (key > 96 && key < 123 || key > 44 && key < 58 || key > 39 && key < 44 || key == 94)
		{
			this->function.insert(this->function.size() + this->move_function, string(1, char(key)));
			if (this->StringWidth(this->function, this->function_font) > this->function_rect.width)
			{
				this->function_begin_element++;
			}
		}
		if (key == 8 && this->function.size() > 0 && this->move_function != -int(this->function.size()))
		{
			this->function.erase(this->function.size() - 1 + this->move_function, 1);
			if(this->function_begin_element != 0)
				this->function_begin_element--;
		}
		if (key == ofKey::OF_KEY_LEFT || key == ofKey::OF_KEY_RIGHT)
		{
			if (key == ofKey::OF_KEY_LEFT)
				this->move_function--;
			if (key == ofKey::OF_KEY_RIGHT)
				this->move_function++;
			if (this->move_function < -int(this->function.size()))
				this->move_function = -int(this->function.size());
			else if (this->move_function > 0)
				this->move_function = 0;
			else
			{
				if (int(this->function.size()) + this->move_function < int(this->function_begin_element) && this->function_begin_element != 0)
					this->function_begin_element--;
				if (this->StringWidth(this->function.substr(this->function_begin_element, this->function.size() + this->move_function - this->function_begin_element), this->function_font) > this->function_rect.width)
					this->function_begin_element++;
			}
		}
	}
	if (this->input_eps)
	{
		this->trig = true;
		if (key > 47 && key < 58 || key == 46)
		{
			this->eps.insert(this->eps.size() + this->move_eps, string(1, char(key)));
			if (this->StringWidth(this->eps, this->function_font) > this->eps_rect.width)
			{
				this->eps_begin_element++;
			}
		}
		if (key == 8 && this->eps.size() > 0 && this->move_eps != -int(this->eps.size()))
		{
			this->eps.erase(this->eps.size() - 1 + this->move_eps, 1);
			if (this->eps_begin_element != 0)
				this->eps_begin_element--;
		}
		if (key == ofKey::OF_KEY_LEFT || key == ofKey::OF_KEY_RIGHT)
		{
			if (key == ofKey::OF_KEY_LEFT)
				this->move_eps--;
			if (key == ofKey::OF_KEY_RIGHT)
				this->move_eps++;
			if (this->move_eps < -int(this->eps.size()))
				this->move_eps = -int(this->eps.size());
			else if (this->move_eps > 0)
				this->move_eps = 0;
			else
			{
				if (int(this->eps.size()) + this->move_eps < int(this->eps_begin_element) && this->eps_begin_element != 0)
					this->eps_begin_element--;
				if (this->StringWidth(this->eps.substr(this->eps_begin_element, this->eps.size() + this->move_eps - this->eps_begin_element), this->function_font) > this->eps_rect.width)
					this->eps_begin_element++;
			}
		}
	}
	if (this->input_a)
	{
		this->trig = true;
		if (key > 47 && key < 58 || key == 46)
		{
			this->a.insert(this->a.size() + this->move_a, string(1, char(key)));
			if (this->StringWidth(this->a, this->function_font) > this->a_rect.width)
			{
				this->a_begin_element++;
			}
		}
		if (key == 8 && this->a.size() > 0 && this->move_a != -int(this->a.size()))
		{
			this->a.erase(this->a.size() - 1 + this->move_a, 1);
			if (this->a_begin_element != 0)
				this->a_begin_element--;
		}
		if (key == ofKey::OF_KEY_LEFT || key == ofKey::OF_KEY_RIGHT)
		{
			if (key == ofKey::OF_KEY_LEFT)
				this->move_a--;
			if (key == ofKey::OF_KEY_RIGHT)
				this->move_a++;
			if (this->move_a < -int(this->a.size()))
				this->move_a = -int(this->a.size());
			else if (this->move_a > 0)
				this->move_a = 0;
			else
			{
				if (int(this->a.size()) + this->move_a < int(this->a_begin_element) && this->a_begin_element != 0)
					this->a_begin_element--;
				if (this->StringWidth(this->a.substr(this->a_begin_element, this->a.size() + this->move_a - this->a_begin_element), this->function_font) > this->a_rect.width)
					this->a_begin_element++;
			}
		}
	}
	if (this->input_quality)
	{
		this->trig = true;
		if (key > 47 && key < 58 || key == 46)
		{
			this->quality.insert(this->quality.size() + this->move_quality, string(1, char(key)));
			if (this->StringWidth(this->quality, this->function_font) > this->quality_rect.width)
			{
				this->quality_begin_element++;
			}
		}
		if (key == 8 && this->quality.size() > 0 && this->move_quality != -int(this->quality.size()))
		{
			this->quality.erase(this->quality.size() - 1 + this->move_quality, 1);
			if (this->quality_begin_element != 0)
				this->quality_begin_element--;
		}
		if (key == ofKey::OF_KEY_LEFT || key == ofKey::OF_KEY_RIGHT)
		{
			if (key == ofKey::OF_KEY_LEFT)
				this->move_quality--;
			if (key == ofKey::OF_KEY_RIGHT)
				this->move_quality++;
			if (this->move_quality < -int(this->quality.size()))
				this->move_quality = -int(this->quality.size());
			else if (this->move_quality > 0)
				this->move_quality = 0;
			else
			{
				if (int(this->quality.size()) + this->move_quality < int(this->quality_begin_element) && this->quality_begin_element != 0)
					this->quality_begin_element--;
				if (this->StringWidth(this->quality.substr(this->quality_begin_element, this->quality.size() + this->move_quality - this->quality_begin_element), this->function_font) > this->quality_rect.width)
					this->quality_begin_element++;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
	this->input_function = false;
	this->input_eps = false;
	this->input_a = false;
	this->input_quality = false;
	if(this->function_rect.inside(ofRectangle(x, y, 1, 1)) && !this->loading)
		this->input_function = true;
	if (this->eps_rect.inside(ofRectangle(x, y, 1, 1)) && !this->loading)
		this->input_eps = true;
	if (this->a_rect.inside(ofRectangle(x, y, 1, 1)) && !this->loading)
		this->input_a = true;
	if (this->quality_rect.inside(ofRectangle(x, y, 1, 1)) && !this->loading)
		this->input_quality = true;
	if (this->button_rect.inside(ofRectangle(x, y, 1, 1)) && !this->loading)
	{	
		if (this->button_active)
		{
			if (!loading)
			{
				generator.setup(467 - 50, 467 - 50, this->num_eps, this->function, this->num_a, this->num_quality);
				std::cout << this->num_eps << " " << this->function << " " << this->num_a << " " << this->num_quality << std::endl;
				loading = true;
				generator.startThread();
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY)
{
	if (this->roots_rect.inside(ofRectangle(x, y, 1, 1)))
	{
		this->scroll_roots -= scrollY;
		if (this->scroll_roots < 0)
			this->scroll_roots = 0;
		if (this->scroll_roots > 4 * (this->roots.size() - 1))
			this->scroll_roots = 4 * (this->roots.size() - 1);
	}
	if (this->singularities_rect.inside(ofRectangle(x, y, 1, 1)))
	{
		this->scroll_singularities -= scrollY;
		if (this->scroll_singularities < 0)
			this->scroll_singularities = 0;
		if (this->scroll_singularities > 4 * (this->singularities.size() - 1))
			this->scroll_singularities = 4 * (this->singularities.size() - 1);
	}
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

void ofApp::DrawFunctionInput(int x, int y, int width, int height)
{
	ofSetColor(this->form_color);
	ofDrawRectangle(x, y, width, height);
	ofSetColor(this->text_color);
	function_font.drawString("f(z) = ", x + 2 * this->ofset, y + height / 2. - 0.3 * this->function_font.getSize() + function_font.stringHeight("(") / 2);
	ofSetColor(this->wait_color_function);
	this->function_rect = ofRectangle(x + 0.3 * this->function_font.getSize() + this->ofset + function_font.stringWidth("f(z) = "), y + height/2. - function_font.stringHeight("(") / 2 - this->ofset, width - 2 * this->ofset - function_font.stringWidth("f(z) = ") - 0.3 * this->function_font.getSize(), function_font.stringHeight("(") + 2 * this->ofset);
	ofDrawRectangle(this->function_rect.x, this->function_rect.y, this->function_rect.width, this->function_rect.height);
}

void ofApp::DrawArgumentsInput(int x, int y, int width, int height)
{
	int width_input = width / 3. - (this->symbols_font.stringWidth("e =") + this->function_font.stringWidth("a =") + this->function_font.stringWidth("quality =") + 2 * this->function_font.stringWidth(",")) / 3. - this->ofset * 3;
	ofSetColor(this->form_color);
	ofDrawRectangle(x, y, width, height);
	ofSetColor(this->text_color);
	this->symbols_font.drawString("e = ", x + this->ofset, y + height / 2. + this->symbols_font.stringHeight("e") / 2);
	ofSetColor(this->wait_color_eps);
	this->eps_rect = ofRectangle(x + this->ofset + this->symbols_font.stringWidth("e = "), y + height/2. - this->symbols_font.stringHeight("e") / 2 - this->ofset, width_input, this->symbols_font.stringHeight("e") + 2 * this->ofset);
	ofDrawRectangle(this->eps_rect.x, this->eps_rect.y, this->eps_rect.width, this->eps_rect.height);
	int add_width = this->symbols_font.stringWidth("e = ") + width_input + this->ofset;
	ofSetColor(this->text_color);
	this->function_font.drawString(",", x + this->ofset + add_width, y + height / 2. + this->function_font.stringHeight("e") / 2);
	add_width += this->ofset + this->function_font.stringWidth(",");
	ofSetColor(this->text_color);
	this->function_font.drawString("a = ", x + this->ofset + add_width, y + height / 2. + this->function_font.stringHeight("a") / 2);
	add_width += this->function_font.stringWidth("a = ");
	ofSetColor(this->wait_color_a);
	this->a_rect = ofRectangle(x + this->ofset + add_width, y + height / 2. - this->function_font.stringHeight("a") / 2 - this->ofset, width_input, this->function_font.stringHeight("a") + 2 * this->ofset);
	ofDrawRectangle(this->a_rect.x, this->a_rect.y, this->a_rect.width, this->a_rect.height);
	add_width += width_input + this->ofset;
	ofSetColor(this->text_color);
	this->function_font.drawString(",", x + this->ofset + add_width, y + height / 2. + this->function_font.stringHeight(",") / 2);
	add_width += this->ofset + this->function_font.stringWidth(",");
	ofSetColor(this->text_color);
	this->function_font.drawString("quality = ", x + this->ofset + add_width, y + height / 2. + this->function_font.stringHeight("(") / 2 - 0.3 * this->function_font.getSize());
	add_width += this->function_font.stringWidth("quality = ");
	ofSetColor(this->wait_color_quality);
	this->quality_rect = ofRectangle(x + this->ofset + add_width, y + height / 2. - this->function_font.stringHeight("(") / 2 - this->ofset, width_input, this->function_font.stringHeight("(") + 2 * this->ofset);
	ofDrawRectangle(this->quality_rect.x, this->quality_rect.y, this->quality_rect.width, this->quality_rect.height);
}

void ofApp::DrawRoots(int x, int y, int width, int height)
{
	ofSetColor(this->form_color);
	this->roots_rect = ofRectangle(x, y, width, height);
	ofDrawRectangle(x, y, width, height);
	height -= 2 * function_font.stringHeight("1");
	y += function_font.stringHeight("1");
	int i = 1;
	for (auto root : this->roots)
	{
		ofSetColor(this->text_color.r, this->text_color.g, this->text_color.b, 2 * function_font.stringHeight("1") * i - this->scroll_roots * function_font.stringHeight("1") / 2 > 0 ? -(2 * function_font.stringHeight("1") * i - this->scroll_roots * function_font.stringHeight("1") / 2 - height) * 255. / (0.5 * height) : 0);
		//this->function_font.drawString(root, x + width/2. - this->function_font.stringWidth(root)/2, y + 2*function_font.stringHeight("1")*i - 0.3 * this->function_font.getSize() - this->scroll_roots * function_font.stringHeight("1") / 2);
		this->DrawStringInRect(x + this->ofset, y + 2 * function_font.stringHeight("1") * (i - 1) - this->scroll_roots * function_font.stringHeight("1") / 2 + this->ofset, width - 2*this->ofset, 2 * function_font.stringHeight("1") - this->ofset, root, this->dynamic_font);
		i++;
	}
	ofSetColor(this->form_color);
	ofDrawRectangle(x, y - this->function_font.stringHeight("1"), width, this->function_font.stringHeight("1"));
}

void ofApp::DrawSingularities(int x, int y, int width, int height)
{
	ofSetColor(this->form_color);
	this->singularities_rect = ofRectangle(x, y, width, height);
	ofDrawRectangle(x, y, width, height);
	height -= 2 * function_font.stringHeight("1");
	y += function_font.stringHeight("1");
	int i = 1;
	for (auto singularity : this->singularities)
	{
		ofSetColor(this->text_color.r, this->text_color.g, this->text_color.b, 2 * function_font.stringHeight("1") * i - this->scroll_singularities * function_font.stringHeight("1") / 2 > 0 ? -(2 * function_font.stringHeight("1") * i - this->scroll_singularities * function_font.stringHeight("1") / 2 - height) * 255. / (0.5 * height) : 0);
		//this->function_font.drawString(singularity, x + width / 2. - this->function_font.stringWidth(singularity) / 2, y + 2 * function_font.stringHeight("1") * i - 0.3 * this->function_font.getSize() - this->scroll_singularities * function_font.stringHeight("1") / 2);
		this->DrawStringInRect(x + this->ofset, y + 2 * function_font.stringHeight("1") * (i - 1) - this->scroll_singularities * function_font.stringHeight("1") / 2 + this->ofset, width - 2 * this->ofset, 2 * function_font.stringHeight("1") - this->ofset, singularity, this->dynamic_font);
		i++;
	}
	ofSetColor(this->form_color);
	ofDrawRectangle(x, y - this->function_font.stringHeight("1"), width, this->function_font.stringHeight("1"));
}

void ofApp::DrawSubtitle(int x, int y, int width, int height, string subtitle)
{
	ofSetColor(this->text_color);
	this->subtitle_font.drawString(subtitle, x + width / 2. - this->subtitle_font.stringWidth(subtitle) / 2, y + height / 2. - 0.3 * this->subtitle_font.getSize() + this->subtitle_font.stringHeight("(") / 2);
	ofSetLineWidth(2);
	ofDrawLine(x, y + height, x + width, y + height);
}

void ofApp::DrawNotification(int x, int y, string notification)
{
	float width = this->ofset * 2 + this->notification_font.stringWidth(notification);
	float height = -this->ofset * 2 + -this->notification_font.stringHeight(notification);
	ofSetColor(ofColor::gray);
	ofDrawRectangle(x, y, width, height);
	ofSetColor(ofColor::white);
	this->notification_font.drawString(notification, x + width / 2. - this->notification_font.stringWidth(notification) / 2, y + height / 2. + this->notification_font.stringHeight(notification) / 2);
}

void ofApp::DrawStringInRect(int x, int y, int width, int height, string s, ofTrueTypeFont font, StringPosition pos)
{
	ofPushMatrix();
	ofTranslate(x + width / 2., y + height / 2.);
	double k = width / font.stringWidth(s);
	double m = height / font.stringHeight("(");
	double w = k < m ? k : m;
	ofScale(w, w);
	switch (pos)
	{
	case StringPosition::CENTER:
		font.drawString(s, -font.stringWidth(s) / 2, font.stringHeight("(") / 2 + font.getDescenderHeight());
		break;
	case StringPosition::LEFT:
		font.drawString(s, -width / 2. / w, font.stringHeight("(") / 2 + font.getDescenderHeight());
		break;
	case StringPosition::RIGHT:
		font.drawString(s, width / 2. / w - font.stringWidth(s), font.stringHeight("(") / 2 + font.getDescenderHeight());
		break;
	default:
		font.drawString(s, -font.stringWidth(s) / 2, font.stringHeight("(") / 2 + font.getDescenderHeight());
		break;
	}
	ofPopMatrix();
}

void ofApp::DrawFrame(int x, int y, int width, int height, int border)
{
	ofNoFill();
	ofSetColor(this->text_color);
	ofSetLineWidth(2);
	string border_variable;
	try
	{
		string num = std::to_string(std::stod(this->a));
		int i = num.size() - 1;
		while ((num.at(i) == '0' || num.at(i) == '.') && i > 0 && num.find('.') != string::npos)
			num.erase(i--, 1);
		border_variable = this->a.empty() ? "a" : num;
	}
	catch (const std::invalid_argument& ia)
	{
		border_variable = "a";
	}
	ofDrawRectangle(x + border, y, width - border, height - border);
	ofDrawLine(x + border, y, x + 3. * border / 4., y);
	ofDrawLine(x + border, y + (height - border) / 2., x + 3. * border / 4., y + (height - border) / 2.);
	ofDrawLine(x + border, y + height - border, x + 3. * border / 4., y + height - border);
	ofDrawLine(x + border, y + height - border, x + border, y + height - 3. * border / 4.);
	ofDrawLine(x + (width + border)/2., y + height - border, x + (width + border) / 2., y + height - 3. * border / 4.);
	ofDrawLine(x + width, y + height - border, x + width, y + height - 3. * border / 4.);
	this->DrawStringInRect(x, y - 3. * border / 8., 3. * border / 4., 3. * border / 4., border_variable, this->dynamic_font);
	this->DrawStringInRect(x, y - 3. * border / 8. + (height - border)/2., 3. * border / 4., 3. * border / 4., "0", this->dynamic_font);
	this->DrawStringInRect(x, y - 3. * border / 8. + height - border, 3. * border / 4., 3. * border / 4., '-' + border_variable, this->dynamic_font);
	this->DrawStringInRect(x + border - 3. * border / 8., y + height - border + 1/4.*border, 3. * border / 4., 3. * border / 4., '-' + border_variable, this->dynamic_font);
	this->DrawStringInRect(x + border - 3. * border / 8. + (width - border)/2., y + height - border + 1 / 4. * border, 3. * border / 4., 3. * border / 4., "0", this->dynamic_font);
	this->DrawStringInRect(x + border - 3. * border / 8. + width - border, y + height - border + 1 / 4. * border, 3. * border / 4., 3. * border / 4., border_variable, this->dynamic_font);
	ofFill();
}

void ofApp::DrawLegend(int x, int y, int width, int height, string name_legend)
{
	ofSetColor(this->text_color);
	ofDrawRectangle(x, y + height/2., width, height/2.);
	this->DrawStringInRect(x, y, width, height / 2., name_legend, this->title_font);
	ofSetColor(ofColor::white);
	this->DrawStringInRect(x + height / 4., y + height / 2., width, height / 4., " - roots", this->subtitle_font, StringPosition::LEFT);
	this->DrawStringInRect(x + height / 4., y + 3. * height / 4., width, height / 4., " - singularities", this->subtitle_font, StringPosition::LEFT);
	this->DrawRoot(x + height / 8., y + height / 2. + height / 8., height / 4.);
	this->DrawSingularity(x + height / 8., y + 3. * height / 4. + height / 8., height / 4.);
}

void ofApp::DrawRoot(int x, int y, int d)
{
	ofSetCircleResolution(50);
	ofDrawCircle(x, y, d*0.5/2.);
}

void ofApp::DrawSingularity(int x, int y, int d)
{
	ofDrawLine(x - d * 0.5 / 2., y - d * 0.5 / 2., x + d * 0.5 / 2., y + d * 0.5 / 2.);
	ofDrawLine(x + d * 0.5 / 2., y - d * 0.5 / 2., x - d * 0.5 / 2., y + d * 0.5 / 2.);
}

float ofApp::StringWidth(string s, ofTrueTypeFont font)
{
	float width = 0;
	for (auto symbol : s)
		width += font.stringWidth(string(1, symbol));
	return width;
}

void ofApp::DrawTitle(int x, int y, int width, int height, string title)
{
	ofSetColor(this->text_color);
	this->title_font.drawString(title, x + width/2. - this->title_font.stringWidth(title)/2, y + height / 2. - 0.3 * this->title_font.getSize() + this->title_font.stringHeight("(") / 2);
	ofSetLineWidth(2);
	ofDrawLine(x, y + height, x + width, y + height);
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
