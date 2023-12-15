#include "EventReader.h"
#include <cassert>

namespace my_ns
{
	EventReader::EventReader(sf::Window& window_ref) noexcept
		: window(window_ref)
	{
	}

	void EventReader::Tick()
	{
		for (sf::Event& event_elenent : event_queue)
		{
			event_table[static_cast<int>(event_elenent.type)].clear();
		}

		event_queue.clear();

		sf::Event event_holder;
		while (window.pollEvent(event_holder))
		{
			event_table[static_cast<int>(event_holder.type)].push_back(static_cast<int>(event_queue.size()));
			event_queue.push_back(event_holder);
		}
	}

	int EventReader::CountEvents(sf::Event::EventType type) const noexcept
	{
		return static_cast<int>(event_table[static_cast<int>(type)].size());
	}

	const sf::Event& EventReader::GetEvent(sf::Event::EventType type, int index) const noexcept
	{
		return event_queue[event_table[static_cast<int>(type)][index]];
	}

	Button::Button(const EventReader& parent_ref)
		: parent(parent_ref)
	{
	}

	bool Button::IsDown() const noexcept
	{
		if (is_fast_changed_twice)
		{
			return !is_down;
		}
		return is_down;
	}

	bool Button::IsPushStart() const noexcept
	{
		if (is_fast_changed_twice)
		{
			return true;
		}
		return is_changed && is_down;
	}

	bool Button::IsPushEnd() const noexcept
	{
		if (is_fast_changed_twice)
		{
			return true;
		}
		return is_changed && !is_down;
	}

	void Button::Tick()
	{
		is_fast_changed_twice = false;
		is_changed = false;

		TickAdditional();
	}

	ButtonKB::ButtonKB(const EventReader& parent_ref, ScanCode button_val)
		: Button(parent_ref)
		, button(button_val)
	{
	}

	void ButtonKB::TickAdditional()
	{
		bool is_press_begin = false;
		const sf::Event* last_begin;
		bool is_press_end = false;
		const sf::Event* last_end;

		for (int i = parent.CountEvents(sf::Event::EventType::KeyPressed) - 1; i >= 0; --i)
		{
			last_begin = &parent.GetEvent(sf::Event::EventType::KeyPressed, i);
			if (last_begin->key.scancode == button)
			{
				is_press_begin = true;
				break;
			}
		}

		for (int i = parent.CountEvents(sf::Event::EventType::KeyReleased) - 1; i >= 0; --i)
		{
			last_end = &parent.GetEvent(sf::Event::EventType::KeyReleased, i);
			if (last_end->key.scancode == button)
			{
				is_press_end = true;
				break;
			}
		}

		if (is_press_begin && is_press_end)
		{
			is_fast_changed_twice = true;
			is_down = last_begin > last_end;

			return;
		}

		if (is_press_begin)
		{
			is_changed = true;
			is_down = true;

			return;
		}

		if (is_press_end)
		{
			is_changed = true;
			is_down = false;
		}
	}

	ButtonMouse::ButtonMouse(const EventReader& parent_ref, MButton button_val)
		: Button(parent_ref)
		, button(button_val)
	{
	}

	void ButtonMouse::TickAdditional()
	{
		bool is_press_begin = false;
		const sf::Event* last_begin;
		bool is_press_end = false;
		const sf::Event* last_end;

		for (int i = parent.CountEvents(sf::Event::EventType::MouseButtonPressed) - 1; i >= 0; --i)
		{
			last_begin = &parent.GetEvent(sf::Event::EventType::MouseButtonPressed, i);
			if (last_begin->mouseButton.button == button)
			{
				is_press_begin = true;
				break;
			}
		}

		for (int i = parent.CountEvents(sf::Event::EventType::MouseButtonReleased) - 1; i >= 0; --i)
		{
			last_end = &parent.GetEvent(sf::Event::EventType::MouseButtonReleased, i);
			if (last_end->mouseButton.button == button)
			{
				is_press_end = true;
				break;
			}
		}

		if (is_press_begin && is_press_end)
		{
			is_fast_changed_twice = true;
			is_down = last_begin > last_end;

			return;
		}

		if (is_press_begin)
		{
			is_changed = true;
			is_down = true;

			return;
		}

		if (is_press_end)
		{
			is_changed = true;
			is_down = false;
		}
	}

	ButtonScroll::ButtonScroll(const EventReader& parent_ref, Direction button_val)
		: Button(parent_ref)
		, button(button_val)
	{
	}

	void ButtonScroll::TickAdditional()
	{

		for (int i = parent.CountEvents(sf::Event::EventType::MouseWheelScrolled) - 1; i >= 0; --i)
		{
			auto&& scroll = parent.GetEvent(sf::Event::EventType::MouseWheelScrolled, i).mouseWheelScroll;

			switch (button)
			{
			case Direction::Up:
			{
				if (scroll.delta > 0 && scroll.wheel == sf::Mouse::Wheel::VerticalWheel)
				{
					is_fast_changed_twice = true;
					return;
				}
			}
			break;
			case Direction::Down:
			{
				if (scroll.delta > 0 && scroll.wheel == sf::Mouse::Wheel::VerticalWheel)
				{
					is_fast_changed_twice = true;
					return;
				}
			}
			break;
			case Direction::Left:
			{
				if (scroll.delta > 0 && scroll.wheel == sf::Mouse::Wheel::VerticalWheel)
				{
					is_fast_changed_twice = true;
					return;
				}
			}
			break;
			case Direction::Right:
			{
				if (scroll.delta > 0 && scroll.wheel == sf::Mouse::Wheel::VerticalWheel)
				{
					is_fast_changed_twice = true;
					return;
				}
			}
			break;
			}
		}
	}
}