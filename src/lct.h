#pragma once

#include <vector>

//-//

template <class LINE, bool COMPRESSED, class...> class LCT;

template <class LINE, bool COMPRESSED, class A> class LCT <LINE, COMPRESSED, A> {
	
public:
	
	LCT(const std::vector <A>& _cm) :
	m_cm(_cm)
	{
		static_assert(COMPRESSED == true);
		m_size = 1;
		while (m_size < _cm.size()) {
			m_size <<= 1;
		}
		m_lines.resize(m_size << 1, { false, LINE() });
	}
	
	void insert(const LINE& line) {
		m_insert(line, 0, 0, m_cm.size());
	}
	
	template <class B> decltype (((LINE*) nullptr)->f(NULL))
	query(const B& value) {
		return m_query(value, 0, 0, m_cm.size());
	}
	
private:
	
	size_t m_size;
	std::vector <std::pair <bool, LINE>> m_lines;
	
	std::vector <A> m_cm;
	
private:
	
	void m_insert(LINE line, size_t now, size_t l, size_t r) {
		if (l >= m_cm.size()) {
			return;
		}
		size_t mid = (l + r) >> 1;
		bool fl = !m_lines[now].first || line.f(m_cm[l]) < m_lines[now].second.f(m_cm[l]);
		bool ml = !m_lines[now].first || line.f(m_cm[mid]) < m_lines[now].second.f(m_cm[mid]);
		if (ml) {
			std::swap(line, m_lines[now].second);
			if (!m_lines[now].first) {
				m_lines[now].first = true;
				return;
			}
			m_lines[now].first = true;
		}
		if (r <= l + 1) {
			return;
		}
		if (fl ^ ml) {
			m_insert(line, (now << 1) + 1, l, mid);
		} else {
			m_insert(line, (now << 1) + 2, mid + 1, r);
		}
	}
	
	template <class B> decltype (((LINE*) nullptr)->f(NULL))
	m_query(const B& value, size_t now, size_t l, size_t r) {
		if (!m_lines[now].first || r <= l + 1) {
			return m_lines[now].first ? m_lines[now].second.f(value) : LINE::inf;
		}
		size_t mid = (l + r) >> 1;
		if (value < m_cm[mid]) {
			return std::min(m_lines[now].second.f(value), m_query(value, (now << 1) + 1, l, mid));
		}
		return std::min(m_lines[now].second.f(value), m_query(value, (now << 1) + 2, mid + 1, r));
	}
	
};

template <class LINE, bool COMPRESSED> class LCT <LINE, COMPRESSED> {
	
public:
	
	LCT(size_t _size) {
		static_assert(COMPRESSED == false);
		m_size = 1;
		while (m_size < _size) {
			m_size <<= 1;
		}
		m_lines.resize(m_size << 1, { false, LINE() });
	}
	
	void insert(const LINE& line) {
		m_insert(line, 0, 0, m_size);
	}
	
	template <class B> decltype (((LINE*) nullptr)->f(NULL))
	query(const B& value) {
		return m_query(value, 0, 0, m_size);
	}
	
private:
	
	size_t m_size;
	std::vector <std::pair <bool, LINE>> m_lines;
	
private:
	
	void m_insert(LINE line, size_t now, size_t l, size_t r) {
		size_t mid = (l + r) >> 1;
		bool fl = !m_lines[now].first || line.f(l) < m_lines[now].second.f(l);
		bool ml = !m_lines[now].first || line.f(mid) < m_lines[now].second.f(mid);
		if (ml) {
			std::swap(line, m_lines[now].second);
			if (!m_lines[now].first) {
				m_lines[now].first = true;
				return;
			}
			m_lines[now].first = true;
		}
		if (r <= l + 1) {
			return;
		}
		if (fl ^ ml) {
			m_insert(line, (now << 1) + 1, l, mid);
		} else {
			m_insert(line, (now << 1) + 2, mid + 1, r);
		}
	}
	
	template <class B> decltype (((LINE*) nullptr)->f(NULL))
	m_query(const B& value, size_t now, size_t l, size_t r) {
		if (!m_lines[now].first || r <= l + 1) {
			return m_lines[now].first ? m_lines[now].second.f(value) : LINE::inf;
		}
		size_t mid = (l + r) >> 1;
		if (value < mid) {
			return std::min(m_lines[now].second.f(value), m_query(value, (now << 1) + 1, l, mid));
		}
		return std::min(m_lines[now].second.f(value), m_query(value, (now << 1) + 2, mid + 1, r));
	}
	
};
