#pragma once
#include <SFML/Window.hpp>
#include <vector>
#include <array>

namespace my_ns
{
	class Button;

	class EventReader
	{
		sf::Window& window;

		static constexpr int event_type_count = sf::Event::EventType::Count;
		std::vector<sf::Event> event_queue;
		std::array<std::vector<int>, event_type_count> event_table;

	public:

		explicit EventReader(sf::Window& window_ref) noexcept;

		// clears previous events and reads new events;
		void Tick();

		// counts events of choosen type
		int CountEvents(sf::Event::EventType type) const noexcept;

		// use CountEvents before. can cause "out of bounds" error, that will fail assetrion (or will cause UB in release version)
		// order of events can be determined by adress of value
		const sf::Event& GetEvent(sf::Event::EventType type, int index) const noexcept;
	};

	class Button
	{

	protected:

		bool is_down = false;
		bool is_changed = false;

		//  twice, or multiple times, even times.
		bool is_fast_changed_twice = false;

		const EventReader& parent;

		// is called inside of Tick arter nulling "is_changed" and "is_fast_changed_twice"
		virtual void TickAdditional() = 0;

	public:

		explicit Button(const EventReader& parent_ref);

		bool IsDown() const noexcept;
		bool IsPushStart() const noexcept;
		bool IsPushEnd() const noexcept;

		void Tick();
	};

	class ButtonKB : public Button
	{

		const sf::Keyboard::Scancode button;

	public:

		using ScanCode = sf::Keyboard::Scancode;

		ButtonKB(const EventReader& parent_ref, ScanCode button_val);

	private:

		void TickAdditional() override;

	};

	class ButtonMouse : public Button
	{

		const sf::Mouse::Button button;

	public:

		using MButton = sf::Mouse::Button;

		ButtonMouse(const EventReader& parent_ref, MButton button_val);

	private:

		void TickAdditional() override;

	};

	class ButtonScroll : public Button
	{

	public:

		enum class Direction
		{
			Up,
			Down,
			Left,
			Right,
		};

	private:

		const Direction button;

	public:

		ButtonScroll(const EventReader& parent_ref, Direction button_val);

	private:

		void TickAdditional() override;

	};
}
