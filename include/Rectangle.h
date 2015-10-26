#pragma once

class Rectangle
{
public:
	Rectangle(int x, int y, int height, int width);

	int getX() const;
	int getY() const;
	int getHeight() const;
	int getWidth() const;

	void setX(int x);
	void setY(int y);
	void setHeight(int height);
	void setWidth(int width);

protected:
	int m_x;
	int m_y;
	int m_height;
	int m_width;

};

