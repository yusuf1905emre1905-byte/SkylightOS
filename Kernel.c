/* SkylightOS minimal grafik kernel, tam kod */
/* Splash ekranı, profil animasyonu, masaüstü, sol altta "Skylight" butonu
 * ve tıklanınca açılan sistem uygulamaları menüsü, menu'den "Kids Home" dahil her uygulamayı
 * tıklayınca ana ekrana açar - mouse input kodu EKSİK, ama GUI/uygulama mantığı TAMDIR
 */

#define WIDTH 320
#define HEIGHT 200

unsigned char *vmem = (unsigned char *)0xA0000;
int menu_open = 0;         // Start menü açık mı
int selected_app = 0;      // Uygulama seçildi mi (1...NUM_APPS)

const char *apps[] = {
    "File Explorer",
    "AI Assistant",
    "App Center",
    "Game Console",
    "Kids Home"        // Son uygulama
};
#define NUM_APPS 5

void draw_box(int x, int y, int w, int h, unsigned char color) {
    for (int yy = y; yy < y + h; yy++)
        for (int xx = x; xx < x + w; xx++)
            vmem[yy * WIDTH + xx] = color;
}
void draw_text(int x, int y, const char *str, unsigned char color) {
    // Sadece sembolik kutu
    draw_box(x, y, 90, 12, color);
    // Gerçek font veya harf için ekstra fonksiyon eklenmeli
}

void splash_screen() {
    asm volatile ("mov $0x13, %al\nint $0x10");
    draw_box(110, 40, 100, 100, 9);          // Logo kutusu
    draw_text(115, 150, "SkylightOS", 15);   // Başlık
    for (int i=0; i<80; i+=4) { draw_box(120,170,i,10,10); for (volatile int d=0; d<28000; d++);}
}
void draw_avatar() {
    for (int sz=10; sz<=40; sz+=2) {
        draw_box(120+20-sz/2, 60+20-sz/2, sz, sz, 11);
        for (volatile int d=0; d<12000; d++);
    }
    draw_box(120, 60, 40, 40, 11);
}
void draw_skylight_logo() {
    // Sol alt Skylight tuşu (Windows tuşu gibi)
    draw_box(8, HEIGHT-28, 34, 18, 9);
    draw_text(16, HEIGHT-24, "*", 15);       // Basit logo
}
void draw_start_menu() {
    draw_box(8, HEIGHT-80, 120, 62, 7);      // Menü kutusu
    draw_text(16, HEIGHT-76, "System Apps", 15);
    for (int i=0; i<NUM_APPS; i++)
        draw_text(20, HEIGHT-64 + i*12, apps[i], 9);
}
void show_app(int idx) {
    draw_box(60,80,200,40,11);                           // Uygulama arkaplanı
    if (idx==0) draw_text(120,94,"[File Explorer]",15);
    if (idx==1) draw_text(120,94,"[AI Assistant]",15);
    if (idx==2) draw_text(120,94,"[App Center]",15);
    if (idx==3) draw_text(120,94,"[Game Console]",15);
    if (idx==4) draw_text(120,94,"[Kids Home]",15);
}

void main_menu() {
    draw_box(0,0,WIDTH,HEIGHT,3);                   // Masaüstü
    draw_text(40,50,"SkylightOS Desktop",15);
    draw_skylight_logo();                           // Sol altta Skylight butonu
    if (menu_open)
        draw_start_menu();                          // Başlat menüsü açık ise
    if (selected_app > 0)
        show_app(selected_app-1);                   // Bir uygulama seçildiyse aç
}

// Tıklama algılama (sanal/demoda; gerçek mouse inputla birleştirmen gerek)
void check_click(int x, int y) {
    // Sol alt Skylight tuşu
    if (x>=8 && x<=42 && y>=HEIGHT-28 && y<=HEIGHT-10) {
        menu_open = !menu_open;
        selected_app = 0;
        main_menu();
    }
    // Menüde uygulama seçim satırları
    if (menu_open && x>=8 && x<=128 && y>=HEIGHT-64 && y<=HEIGHT-64+NUM_APPS*12) {
        int sel = (y-(HEIGHT-64))/12;
        if (sel>=0 && sel<NUM_APPS) {
            selected_app = sel+1;
            menu_open = 0;
            main_menu();
        }
    }
}

void SkylightOS_main() {
    splash_screen();
    draw_avatar();
    main_menu();
    while (1) {
        // Gerçek mouse input ile x, y koordinatını bul, check_click(x,y) ile işlem yap
        // Örneğin BIOS mouse input modülü eklenirse burada ana GUI döngüsü yapılır
    }
}
void _start() { SkylightOS_main(); }
