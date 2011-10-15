#ifndef EG_GUI_GUI_H
#define EG_GUI_GUI_H

#include <string>

#include "../Math/Math.h"
#include "../Utility/Dictionary.h"

namespace EG{
	namespace GUI{
		// Should this be in it's own namespace?
		// Probably...
		class WidgetStyle{
			public:
				WidgetStyle(void);
				~WidgetStyle(void);

				void SetPadding(glm::vec4 _padding);
				void SetMargin(glm::vec4 _margin);
				void SetFrontColor(glm::vec4 _front_color);
				void SetBackColor(glm::vec4 _back_color);
				void SetBorderSize(float _border_size);
				void SetBorderColor(glm::vec4 _border_color);

				glm::vec4 GetPadding(void);
				glm::vec4 GetMargin(void);
				glm::vec4 GetFrontColor(void);
				glm::vec4 GetBackColor(void);
				float GetBorderSize(void);
				glm::vec4 GetBorderColor(void);
			private:
				glm::vec4 padding;
				glm::vec4 margin;

				glm::vec4 front_color;
				glm::vec4 back_color;

				float border_size;
				glm::vec4 border_color;
		};

		class Widget{
			public:
				enum WidgetType{
					GUI_WIDGET = 0,
					GUI_LABEL = 1,
					GUI_BUTTON = 2,
					GUI_INPUT = 3,
					GUI_CHECKBOX = 4,
					GUI_SELECT = 5,
					GUI_SLIDER = 6,
					GUI_PANEL = 7
				};

				Widget(void);
				~Widget(void);

				void SetPosition(glm::vec2 _position);
				void SetSize(glm::vec2 _size);
				void SetWidgetId(std::string _widget_id);
				void SetWidgetStyle(WidgetStyle *_widget_style);

				glm::vec2 GetPosition(void);
				glm::vec2 GetSize(void);
				std::string GetWidgetId(void);
				WidgetType GetWidgetType(void);
				WidgetStyle *GetWidgetStyle(void);
			protected:
				glm::vec2 position;
				glm::vec2 size;
				std::string widget_id;
				WidgetType widget_type;
				WidgetStyle *widget_style;
		};

		class Label : public Widget{
			public:
				Label(std::string _text = "default");
				~Label(void);

				void SetText(std::string _text);
				std::string GetText(void);
			private:
				std::string text;
		};

		class Button : public Widget{
			public:
				Button(std::string _text = "default");
				~Button(void);

				void SetText(std::string _text);
				std::string GetText(void);
			private:
				std::string text;
				// action
		};

		class Input : public Widget{
			public:
				Input(std::string _text = "", unsigned int _max_characters = 100);
				~Input(void);

				void SetMaxCharacters(unsigned int _max_characters);
				void SetText(std::string _text);

				unsigned int GetMaxCharacters(void);
				std::string GetText(void);
			private:
				unsigned int max_characters;
				std::string text;
		};

		class Checkbox : public Widget{
			public:
				Checkbox(bool _checked = false);
				~Checkbox(void);

				void SetChecked(bool _checked);
				bool GetChecked(void);

				void Toggle(void);
			private:
				bool checked;
		};

		class Select : public Widget{
			public:
				Select(void);
				~Select(void);

				void SetSelectedValue(std::string _selected_value);
				void AddEntry(std::string _value, std::string _display_value);

				std::string GetSelectedValue(void);
			private:
				std::string selected_value;
				EG::Utility::Dictionary<std::string, std::string> entries;
		};

		class Slider : public Widget{
			public:
				Slider(glm::vec2 _value_range = glm::vec2(0.0f, 100.0f), float _step_size = 0.01f);
				~Slider(void);

				void SetStepSize(float _step_size);
				void SetValueRange(glm::vec2 _value_range);
				void SetValue(float _value);

				float GetStepSize(void);
				glm::vec2 GetValueRange(void);
				float GetValue(void);
			private:
				float value;
				float step_size;
				glm::vec2 value_range;
		};

		// Eventually Add Layouts to this Nonsense!
		class Panel : public Widget{
			public:
				Panel(std::string _title = "default");
				~Panel(void);

				void AddWidget(Widget *_widget);
				void SetTitle(std::string _title);

				std::vector<std::string>::iterator GetWidgetIdsBegin(void);
				std::vector<std::string>::iterator GetWidgetIdsEnd(void);
			private:
				std::string title;
				EG::Utility::Dictionary<std::string, Widget *> widgets;
		};

		class GUI{
			public:
				GUI(void);
				~GUI(void);

				void AddWidget(Widget *_widget);

				std::vector<std::string>::iterator GetWidgetIdsBegin(void);
				std::vector<std::string>::iterator GetWidgetIdsEnd(void);
			private:
				EG::Utility::Dictionary<std::string, Widget *> widgets;
		};

		class GUIRenderer{
			public:
				GUIRenderer(void);
				~GUIRenderer(void);

				void Draw(GUI *gui);
			protected:
		};
	}
}

#endif
