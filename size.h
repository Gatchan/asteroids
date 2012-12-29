#pragma once

struct Size
{
  Size();
  Size(const float width, const float height);
  Size(const Size& other);

  float width;
  float height;
};
