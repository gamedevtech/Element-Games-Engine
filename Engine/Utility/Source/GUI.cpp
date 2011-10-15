#include "../GUI.h"

namespace EG{
	namespace GUI{
		WidgetStyle::WidgetStyle(void){
			padding = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
			margin = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
			front_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			back_color = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
			border_size = 0.0f;
			border_color = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
		}
		WidgetStyle::~WidgetStyle(void){
			//
		}
		void WidgetStyle::SetPadding(glm::vec4 _padding){
			padding = _padding;
		}
		void WidgetStyle::SetMargin(glm::vec4 _margin){
			margin = _margin;
		}
		void WidgetStyle::SetFrontColor(glm::vec4 _front_color){
			front_color = _front_color;
		}
		void WidgetStyle::SetBackColor(glm::vec4 _back_color){
			back_color = _back_color;
		}
		void WidgetStyle::SetBorderSize(float _border_size){
			border_size = _border_size;
		}
		void WidgetStyle::SetBorderColor(glm::vec4 _border_color){
			border_color = _border_color;
		}
		glm::vec4 WidgetStyle::GetPadding(void){
			return padding;
		}
		glm::vec4 WidgetStyle::GetMargin(void){
			return margin;
		}
		glm::vec4 WidgetStyle::GetFrontColor(void){
			return front_color;
		}
		glm::vec4 WidgetStyle::GetBackColor(void){
			return back_color;
		}
		float WidgetStyle::GetBorderSize(void){
			return border_size;
		}
		glm::vec4 WidgetStyle::GetBorderColor(void){
			return border_color;
		}

		Widget::Widget(void){
			widget_type = GUI_WIDGET;
			widget_id = "default";
			position = glm::vec2(0.0f, 0.0f);
			size = glm::vec2(0.0f, 0.0f);
		}
		Widget::~Widget(void){
			//
		}
		void Widget::SetPosition(glm::vec2 _position){
			position = _position;
		}
		void Widget::SetSize(glm::vec2 _size){
			size = _size;
		}
		void Widget::SetWidgetId(std::string _widget_id){
			widget_id = _widget_id;
		}
		void Widget::SetWidgetStyle(WidgetStyle *_widget_style){
			widget_style = _widget_style;
		}
		glm::vec2 Widget::GetPosition(void){
			return position;
		}
		glm::vec2 Widget::GetSize(void){
			return size;
		}
		std::string Widget::GetWidgetId(void){
			return widget_id;
		}
		Widget::WidgetType Widget::GetWidgetType(void){
			return widget_type;
		}
		WidgetStyle *Widget::GetWidgetStyle(void){
			return widget_style;
		}

		Label::Label(std::string _text){
			text = _text;
			widget_type = GUI_LABEL;
		}
		Label::~Label(void){
			//
		}
		void Label::SetText(std::string _text){
			text = _text;
		}
		std::string Label::GetText(void){
			return text;
		}

		Button::Button(std::string _text){
			text = _text;
			widget_type = GUI_BUTTON;
		}
		Button::~Button(void){
			//
		}
		void Button::SetText(std::string _text){
			text = _text;
		}
		std::string Button::GetText(void){
			return text;
		}

		Input::Input(std::string _text, unsigned int _max_characters){
			text = _text;
			max_characters = _max_characters;
			widget_type = GUI_INPUT;
		}
		Input::~Input(void){
			//
		}
		void Input::SetMaxCharacters(unsigned int _max_characters){
			max_characters = _max_characters;
		}
		void Input::SetText(std::string _text){
			text = _text;
		}
		unsigned int Input::GetMaxCharacters(void){
			return max_characters;
		}
		std::string Input::GetText(void){
			return text;
		}

		Checkbox::Checkbox(bool _checked){
			checked = _checked;
			widget_type = GUI_CHECKBOX;
		}
		Checkbox::~Checkbox(void){
			//
		}
		void Checkbox::SetChecked(bool _checked){
			checked = _checked;
		}
		bool Checkbox::GetChecked(void){
			return checked;
		}
		void Checkbox::Toggle(void){
			checked = !checked;
		}

		Select::Select(void){
			widget_type = GUI_SELECT;
		}
		Select::~Select(void){
			//
		}
		void Select::SetSelectedValue(std::string _selected_value){
			selected_value = _selected_value;
		}
		void Select::AddEntry(std::string _value, std::string _display_value){
			entries.Set(_value, _display_value);
		}
		std::string Select::GetSelectedValue(void){
			return selected_value;
		}

		Slider::Slider(glm::vec2 _value_range, float _step_size){
			value_range = _value_range;
			step_size = _step_size;
			widget_type = GUI_SLIDER;
		}
		Slider::~Slider(void){
			//
		}
		void Slider::SetStepSize(float _step_size){
			step_size = _step_size;
		}
		void Slider::SetValueRange(glm::vec2 _value_range){
			value_range = _value_range;
		}
		void Slider::SetValue(float _value){
			value = _value;
		}
		float Slider::GetStepSize(void){
			return step_size;
		}
		glm::vec2 Slider::GetValueRange(void){
			return value_range;
		}
		float Slider::GetValue(void){
			return value;
		}

		Panel::Panel(std::string _title){
			title = _title;
			widget_type = GUI_PANEL;
		}
		Panel::~Panel(void){
			//
		}
		void Panel::AddWidget(Widget *_widget){
			widgets.Set(_widget->GetWidgetId(), _widget);
		}
		void Panel::SetTitle(std::string _title){
			title = _title;
		}
		std::vector<std::string>::iterator Panel::GetWidgetIdsBegin(void){
			return widgets.GetKeysBegin();
		}
		std::vector<std::string>::iterator Panel::GetWidgetIdsEnd(void){
			return widgets.GetKeysEnd();
		}

		GUI::GUI(void){
			//
		}
		GUI::~GUI(void){
			//
		}
		void GUI::AddWidget(Widget *_widget){
			widgets.Set(_widget->GetWidgetId(), _widget);
		}
		std::vector<std::string>::iterator GUI::GetWidgetIdsBegin(void){
			return widgets.GetKeysBegin();
		}
		std::vector<std::string>::iterator GUI::GetWidgetIdsEnd(void){
			return widgets.GetKeysEnd();
		}
	}
}
