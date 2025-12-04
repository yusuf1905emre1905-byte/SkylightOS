// 320x200 grafik modunda SkylightOS açılış ekranı

void draw_box(int x, int y, int w, int h, unsigned char color) {
    unsigned char* vmem = (unsigned char*)0xA0000;
    for (int yy = y; yy < y + h; yy++) {
        for (int xx = x; xx < x + w; xx++) {
            vmem[yy * 320 + xx] = color;
        }
    }
}

// Sembolik yazı (gerçek font eklemesi gerekebilir)
void draw_text(int x, int y, const char* str, unsigned char color) {
    draw_box(x, y, 100, 12, color); // salt placeholder
}

void splash_screen() {
    // Grafik moduna geçiş
    asm volatile ("mov $0x13, %al\nint $0x10");

    // SkylightOS logosu (mavi kutu)
    draw_box(110, 40, 100, 100, 1);

    // SkylightOS yazısı (beyaz kutuda)
    draw_text(105, 150, "SkylightOS", 15);

    // Loading bar animasyonu
    for (int i = 0; i < 80; i+=4) {
        draw_box(120, 170, i, 10, 10);
        for (volatile int d=0; d<30000; d++); // kısa bekletme
    }

    // Sonra ana menüye geçiş
    // ...main_menu();
}
