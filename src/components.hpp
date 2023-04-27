
#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <rack.hpp>
#include "plugin.hpp"

extern Plugin	*pluginInstance;

////////////////////////////////////////////////////////////////////////////////
/// STRUCTURES
////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////
/// SVG COMPONENTS
//////////////////////////////////////////////////

//////////////////////////////
/// KNOBS
//////////////////////////////

struct FlatKnob : app::SvgKnob {
	widget::SvgWidget* bg;

	FlatKnob() {
		minAngle = -0.83 * M_PI;
		maxAngle = 0.83 * M_PI;

		bg = new widget::SvgWidget;
		fb->addChildBelow(bg, tw);
		speed = 2.f;
		shadow->opacity = 0.f;
	}
};

struct KnobSmall : FlatKnob {
	KnobSmall() {
		setSvg(Svg::load(asset::plugin(pluginInstance, "res/Knob-Small.svg")));
		bg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/Knob-Small-bg.svg")));
	}
};

struct KnobMedium : FlatKnob {
	KnobMedium() {
		setSvg(Svg::load(asset::plugin(pluginInstance, "res/Knob-Medium.svg")));
		bg->setSvg(Svg::load(asset::plugin(pluginInstance, "res/Knob-Medium-bg.svg")));
	}
};

//////////////////////////////
/// INLET / OUTLET
//////////////////////////////

struct Outlet : app::SvgPort {
	Outlet() {
		setSvg(Svg::load(asset::plugin(pluginInstance, "res/Outlet.svg")));
		shadow->opacity = 0.0;
	}
};

//////////////////////////////////////////////////
/// MENU COMPONENTS
//////////////////////////////////////////////////

//struct MenuSliderQuantity : Quantity {
//	float		*ptr;
//	float		def;
//	float		min;
//	float		max;
//	std::string	label;
//
//	MenuSliderQuantity(float *ptr, std::string label, float def, float min, float max) {
//		*ptr = def;
//		this->ptr = ptr;
//		this->def = def;
//		this->min = min;
//		this->max = max;
//		this->label = label;
//	}
//
//	void setValue(float value) override {
//		if (value < this->min)
//			value = this->min;
//		if (value > this->max)
//			value = this->max;
//		*(this->ptr) = value; 
//	}
//	float getValue(void) override {
//		return *(this->ptr);
//	}
//	float getDisplayValue(void) override {
//		//return (float)(int)*(this->ptr);
//		return *(this->ptr);
//	}
//	float getMinValue() override { return this->min; }
//	float getMaxValue() override { return this->max; }
//	float getDefaultValue() override {return this->def; }
//	std::string getLabel() override { return label; }
//	std::string getUnit() override { return " "; }
//};

struct MenuTextField : ui::TextField {
	MenuTextField(void) {
		this->box.size = Vec(20, 20);
		this->multiline = false;
		this->setText("Cuicui ?");
	}
};

struct MenuTextFieldLinked : ui::TextField {
	Quantity	*quantity;

	MenuTextFieldLinked(Quantity *quantity, int precision = 2) {
		char	str[32];

		/// INIT STRING
		sprintf(str, "%.*f", precision, quantity->getValue());
		/// INIT TEXT FIELD
		this->box.size.x = 50;
		this->multiline = false;
		this->setText(str);
		this->selectAll();
		this->quantity = quantity;

		APP->event->setSelectedWidget(this);
		this->selectAll();
	}

	void onSelectKey(const SelectKeyEvent &e) override {
		float	value;

		if (e.action == GLFW_PRESS
		&& e.key == GLFW_KEY_ENTER) {
			value = atof((char*)this->getText().c_str());
			this->quantity->setValue(value);
			this->parent->requestDelete();
		} else {
			ui::TextField::onSelectKey(e);
		}
	}
};

struct MenuItemStay : ui::MenuItem {
	std::function<void()>	action_func;

	MenuItemStay(std::string text_left, std::string text_right,
	std::function< void()> action) {
		this->text = text_left;
		this->rightText = text_right;
		this->action_func = action;
	}

	void onButton(const ButtonEvent &e) override {
		if (e.button == GLFW_MOUSE_BUTTON_LEFT) {
			this->action_func();
			e.stopPropagating();
		}
	}
};

struct MenuSlider : ui::Slider {

	//MenuSlider(float *value, std::string label, float def, float min, float max) {
	//	this->quantity = new MenuSliderQuantity(value, label, def, min, max);
	//}
	MenuSlider(ParamQuantity *quantity) {
		this->quantity = quantity;
	}

	~MenuSlider() {
		//delete this->quantity;
	}
};

struct MenuSliderEdit : rack::Widget {
	MenuItem		*item_button;
	MenuSlider		*item_slider;
	Quantity		*quantity;

	MenuSliderEdit(ParamQuantity *quantity) {
		this->box.size.y = 20.0f;
		/// INIT QUANTITY
		this->quantity = quantity;
		/// INIT BUTTON
		//// "▸" //// "▴" //// "▾" //// "◆"
		this->item_button = new MenuItemStay("▾", "",	
			[=]() {
				MenuLabel		*label;
				Menu			*menu;

				/// CREATE MENU
				menu = createMenu();
				menu->box.size.x = 200;
				/// INIT MENU LABEL
				label = new MenuLabel();
				label->text = "Edit value";
				menu->addChild(label);
				/// INIT MENU TEXT FIELD
				menu->addChild(new MenuTextFieldLinked(this->quantity));
			}
		);
		this->item_button->box.size.x = 20.0f;
		this->item_button->box.size.y = 20.0f;
		this->addChild(this->item_button);
		/// INIT SLIDER
		this->item_slider = new MenuSlider(quantity);
		this->item_slider->box.size.x = 180.f;
		this->item_slider->box.size.y = 20.f;
		this->item_slider->box.pos.x = 20.f;
		this->addChild(this->item_slider);
	}

	~MenuSliderEdit() {
	}
};

#endif
