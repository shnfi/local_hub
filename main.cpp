/*
 * MIT License 
 * Copyright (C) 2025 shnfi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy...
 */

#include "src/window.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	Window win;
	win.show();

	return app.exec();
}
