// Copyright (C) 2017 Justin Frank, Jason Waataja
//
// This file is part of TypingTest.
//
// TypingTest is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// TypingTest is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// TypingTest.  If not, see <http:// www.gnu.org/licenses/>.

#include "sticker_buffer.h"

#include <functional>
#include <iostream>
#include <regex>

namespace typingtest {

StickerBuffer::StickerBuffer()
	: TextBuffer{}
	/* stickerTags{1, &StickerBuffer::stickerPairHash} */
{
	signal_insert().connect(sigc::mem_fun(*this,
			&StickerBuffer::onInsertText));
}

void StickerBuffer::onInsertText(const Gtk::TextIter &,
	const Glib::ustring &text, int)
{
	std::cout << "in handle" << std::endl;
	if (text.find(":") != Glib::ustring::npos) {
		std::vector<gunichar> elements;
		for (Gtk::TextIter iter = begin(); iter != end(); ++iter)
			elements.push_back(iter.get_char());
		std::vector<std::pair<int, int>> words = splitChars(elements);
		replaceWords(words);
	}
	/* std::cout << this->getTextWithStickers() << std::endl; */
}

void StickerBuffer::replaceWords(std::vector<std::pair<int, int>> words)
{
	for (size_t i = 0; i < words.size(); ++i) {
		int offset{words[i].first};
		int endOffset{words[i].second};
		Gtk::TextIter startIter{get_iter_at_offset(offset)};
		Gtk::TextIter endIter{get_iter_at_offset(endOffset)};
		std::string text{get_text(startIter, endIter)};
		std::regex re{R"(:(\w*):)"};
		std::smatch match;
		int totalShortened{0};
		while (std::regex_search(text, match, re)) {
			Gtk::TextIter startWordIter{get_iter_at_offset(
				match.position() + offset)};
			Gtk::TextIter endWordIter{get_iter_at_offset(match.position()
				+ match.length() + offset)};
			erase(startWordIter, endWordIter);
			std::string stickerName{match.str(1)};
			int stickerPos = match.position() + offset;
			std::string insertString = "\n";
			Gtk::TextIter stickerIter = get_iter_at_offset(stickerPos);
			if (!stickerIter.starts_line()) {
				stickerPos++;
				insertString += "\n";
			}
			insert(get_iter_at_offset(stickerPos), insertString);

			// For two newline characters.
			int lengthChange = match.length() - insertString.length();

			/* Glib::RefPtr<Gtk::TextTag> stickerTag; */
			/* auto nameIter = std::find_if(stickerTags.begin(), stickerTags.end(), */
			/* 	[&stickerName](std::pair<Glib::RefPtr<Gtk::TextTag>, int> tag) { */
			/* 		return tag.first->property_name() == stickerName; */
			/* 	}); */
			/* if (nameIter != stickerTags.end()) { */
			/* 	stickerTag = nameIter->first; */
			/* 	nameIter->second++; */
			/* } else { */
			/* 	stickerTag = create_tag(stickerName); */
			/* 	stickerTags[stickerTag] = 0; */
			/* } */
			auto pixbuf = engine.createPixbufDefaultSize(stickerName);
			if (pixbuf) {
				insert_pixbuf(get_iter_at_offset(stickerPos), pixbuf);
				--lengthChange;
				/* apply_tag(stickerTag, get_iter_at_offset(stickerPos), */
				/* 	get_iter_at_offset(stickerPos + 1)); */
			}
			totalShortened += lengthChange;
			offset -= lengthChange;
			endOffset -= lengthChange;
			Gtk::TextIter startIter{get_iter_at_offset(offset)};
			Gtk::TextIter endIter{get_iter_at_offset(endOffset)};
			text = get_text(startIter, endIter);
		}
		for (std::vector<std::pair<int, int>>::size_type j = i + 1;
			j < words.size(); ++j) {
			words[i].first -= totalShortened;
			words[i].second -= totalShortened;
		}
	}
}

Glib::RefPtr<StickerBuffer> StickerBuffer::create()
{
	return Glib::RefPtr<StickerBuffer>(new StickerBuffer());
}

std::vector<std::pair<int, int>> StickerBuffer::splitChars(
	const std::vector<gunichar> &elements)
{
	bool inWord{false};
	int startPos;
	int endPos;
	std::vector<std::pair<int, int>> words;
	std::vector<gunichar>::size_type i;
	for (i = 0; i < elements.size(); ++i) {
		if (inWord && elements[i] == UNKNOWN_CHAR) {
			endPos = i;
			inWord = false;
			words.push_back({startPos, endPos});
		} else if (!inWord && elements[i] != UNKNOWN_CHAR) {
			inWord = true;
			startPos = i;
		}
	}
	if (inWord) {
		endPos = i;
		words.push_back({startPos, endPos});
	}
	return words;
}

std::string StickerBuffer::getTextWithStickers()
{
	std::string text;
	/* for (auto iter = begin(); iter != end(); ++iter) { */
	/* 	for (auto pair : stickerTags) { */
	/* 		if (iter.begins_tag(pair.first)) */
	/* 			text += ":" + pair.first->property_name() + ":"; */
	/* 		else */
	/* 			text += iter.get_char(); */
	/* 	} */
	/* } */
	return text;
}

size_t StickerBuffer::stickerPairHash(
	const Glib::RefPtr<Gtk::TextTag>& tag)
{
	std::string name{Glib::ustring{tag->property_name()}};
	return std::hash<std::string>{}(name);
}
} // typingtest
