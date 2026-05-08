#include "ui.h"
#include <stdio.h>

static CalcState state;
static GtkWidget *display_label;

static void update_display(){
    const char *t = calc_get_display(&state);
    gtk_label_set_text(GTK_LABEL(display_label), t);
}

static void digit_clicked(GtkWidget *btn, gpointer data){
  int d = GPOINTER_TO_INT(data);
  g_print("digit %d clicked\n", d);
  calc_enter_digit(&state, d);
  update_display();
}


static void dot_clicked(GtkWidget *btn, gpointer _){
    calc_enter_dot(&state);
    update_display();
}

static void op_clicked(GtkWidget *btn, gpointer data){
    Op op = GPOINTER_TO_INT(data);
    calc_press_op(&state, op);
    update_display();
}

static void eq_clicked(GtkWidget *btn, gpointer _){
    calc_press_equals(&state);
    update_display();
}

static void clear_clicked(GtkWidget *btn, gpointer which){
    if ((int)(intptr_t)which == 1) calc_clear_entry(&state);
    else calc_clear_all(&state);
    update_display();
}

GtkWidget* create_main_window(GtkApplication *app){
    calc_init(&state);

GtkWidget *win = gtk_application_window_new(app);
gtk_window_set_title(GTK_WINDOW(win), "Calculator");
gtk_window_set_default_size(GTK_WINDOW(win), 212, 284);
gtk_window_set_resizable(GTK_WINDOW(win), FALSE);

GtkCssProvider *css = gtk_css_provider_new();
gtk_css_provider_load_from_data(css,
    "window { background-image: linear-gradient(#f6fafd, #d7e1ef); color: #323333; }"
    ".calc-frame { background-image: linear-gradient(#f6fafd, #d7e1ef); border-radius: 4px; padding: 6px; }"
    ".display { background-image: linear-gradient(#d7e1ef, #ffffff); border: 1px solid #cfd8e6; padding: 8px; font-size: 28px; font-weight: 600; }"
    /*"button.calc { border-radius: 6px; background-image: linear-gradient(#f6fafd, #d7e1ef); border: 1px solid #9aa9bb; }"
    "button.op { background-image: linear-gradient(#d7e1ef, #f6fafd); border: 1px solid #7f8fa0; }"
    "button.mem { font-size: 11px; padding: 4px 6px; }"
    ".eq { min-height: 96px; }"*/
    , (gssize)-1, NULL);
gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
    GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
g_object_unref(css);

GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
gtk_container_add(GTK_CONTAINER(win), vbox);
gtk_widget_set_margin_top(vbox, 8);
gtk_widget_set_margin_bottom(vbox, 8);
gtk_widget_set_margin_start(vbox, 8);
gtk_widget_set_margin_end(vbox, 8);

GtkWidget *title_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
GtkWidget *title_label = gtk_label_new("Calculator");
gtk_widget_set_halign(title_label, GTK_ALIGN_START);
gtk_box_pack_start(GTK_BOX(title_row), title_label, FALSE, FALSE, 0);
gtk_box_pack_start(GTK_BOX(vbox), title_row, FALSE, FALSE, 0);

GtkWidget *frame = gtk_frame_new(NULL);
GtkStyleContext *ctx = gtk_widget_get_style_context(frame);
gtk_style_context_add_class(ctx, "calc-frame");
gtk_box_pack_start(GTK_BOX(vbox), frame, FALSE, FALSE, 0);

GtkWidget *display_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
ctx = gtk_widget_get_style_context(display_box);
gtk_style_context_add_class(ctx, "display");
gtk_container_add(GTK_CONTAINER(frame), display_box);

display_label = gtk_label_new("0");
gtk_widget_set_hexpand(display_label, TRUE);
gtk_widget_set_halign(display_label, GTK_ALIGN_END);
gtk_widget_set_valign(display_label, GTK_ALIGN_CENTER);
gtk_widget_set_margin_top(display_label, 6);
gtk_widget_set_margin_bottom(display_label, 6);
gtk_widget_set_margin_start(display_label, 6);
gtk_widget_set_margin_end(display_label, 6);
gtk_label_set_xalign(GTK_LABEL(display_label), 1.0);
gtk_widget_set_size_request(display_label, 260, 48);
gtk_box_pack_start(GTK_BOX(display_box), display_label, TRUE, TRUE, 0);

GtkWidget *grid = gtk_grid_new();
gtk_grid_set_row_spacing(GTK_GRID(grid), 6);
gtk_grid_set_column_spacing(GTK_GRID(grid), 6);
gtk_box_pack_start(GTK_BOX(vbox), grid, FALSE, FALSE, 0);

const char *mems[] = {"MC","MR","MS","M+"};
for (int i=0;i<4;i++){
    GtkWidget *b = gtk_button_new_with_label(mems[i]);
    gtk_widget_set_size_request(b, 76, 28);
    ctx = gtk_widget_get_style_context(b);
    gtk_style_context_add_class(ctx, "mem");
    gtk_style_context_add_class(ctx, "calc");
    gtk_grid_attach(GTK_GRID(grid), b, i, 0, 1, 1);
}

const char *toprow[] = {"\u2190","CE","C","\u00F7"};
for (int c=0;c<4;c++){
    GtkWidget *btn = gtk_button_new_with_label(toprow[c]);
    gtk_widget_set_size_request(btn, 76, 36);
    ctx = gtk_widget_get_style_context(btn);
    gtk_style_context_add_class(ctx, "calc");
    if (c==3) gtk_style_context_add_class(ctx, "op");
    gtk_grid_attach(GTK_GRID(grid), btn, c, 1, 1, 1);
    if (c==0) g_signal_connect(btn, "clicked", G_CALLBACK(clear_clicked), GINT_TO_POINTER(1));
    else if (c==1) g_signal_connect(btn, "clicked", G_CALLBACK(clear_clicked), GINT_TO_POINTER(1));
    else if (c==2) g_signal_connect(btn, "clicked", G_CALLBACK(clear_clicked), GINT_TO_POINTER(0));
    else g_signal_connect(btn, "clicked", G_CALLBACK(op_clicked), GINT_TO_POINTER(OP_DIV));
}

const char *labels[4][4] = {
    {"7","8","9","*"},
    {"4","5","6","-"},
    {"1","2","3","+"},
    {"0",".","","="}
};
for (int r=0;r<4;r++){
    for (int c=0;c<3;c++){
        const char *lab;
        if (r==3 && c==0) lab = "0";
        else lab = labels[r][c];
        GtkWidget *btn = gtk_button_new_with_label(lab);
        gtk_widget_set_size_request(btn, 76, 36);
        ctx = gtk_widget_get_style_context(btn);
        gtk_style_context_add_class(ctx, "calc");
        gtk_grid_attach(GTK_GRID(grid), btn, c, r+2, 1, 1);
        if (lab[0]>='0' && lab[0]<='9'){
            int d = lab[0]-'0';
            g_signal_connect(btn, "clicked", G_CALLBACK(digit_clicked), GINT_TO_POINTER(d));
        } else if (lab[0]=='.'){
            g_signal_connect(btn, "clicked", G_CALLBACK(dot_clicked), NULL);
        }
    }
    const char *oplab = labels[r][3];
    GtkWidget *opbtn = gtk_button_new_with_label(oplab);
    gtk_widget_set_size_request(opbtn, 76, 36);
    ctx = gtk_widget_get_style_context(opbtn);
    gtk_style_context_add_class(ctx, "op");
    gtk_grid_attach(GTK_GRID(grid), opbtn, 3, r+2, 1, 1);
    if (strcmp(oplab, "*") == 0) g_signal_connect(opbtn, "clicked", G_CALLBACK(op_clicked), GINT_TO_POINTER(OP_MUL));
    else if (strcmp(oplab, "+") == 0) g_signal_connect(opbtn, "clicked", G_CALLBACK(op_clicked), GINT_TO_POINTER(OP_ADD));
    else if (strcmp(oplab, "-") == 0) g_signal_connect(opbtn, "clicked", G_CALLBACK(op_clicked), GINT_TO_POINTER(OP_SUB));
    else if (strcmp(oplab, "=") == 0) {
        if (r==2) g_signal_connect(opbtn, "clicked", G_CALLBACK(eq_clicked), NULL);
        else {
            gtk_widget_set_size_request(opbtn, 76, 76);
            g_signal_connect(opbtn, "clicked", G_CALLBACK(eq_clicked), NULL);
            gtk_grid_attach(GTK_GRID(grid), opbtn, 3, 4, 1, 2);
        }
    } else if (strcmp(oplab, "\u00F7") == 0) g_signal_connect(opbtn, "clicked", G_CALLBACK(op_clicked), GINT_TO_POINTER(OP_DIV));
}
gtk_widget_show_all(win);
return win;

}
