#include "ui.h"
#include <stdio.h>

static CalcState state;
static GtkWidget *display_label;
static GtkWidget *display_scroll;

static void scroll_display_to_end(void){
    GtkAdjustment *adj = gtk_scrolled_window_get_hadjustment(GTK_SCROLLED_WINDOW(display_scroll));
    gtk_adjustment_set_value(adj, gtk_adjustment_get_upper(adj));
}

static void update_display(void){
    const char *t = calc_get_display(&state);
    gtk_label_set_text(GTK_LABEL(display_label), t);
    /* scroll after GTK has reflowed the label's new size */
    g_idle_add((GSourceFunc)scroll_display_to_end, NULL);
}

static void digit_clicked(GtkWidget *btn, gpointer data){
    int d = GPOINTER_TO_INT(data);
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

static void negate_clicked(GtkWidget *btn, gpointer _){
    calc_negate(&state);
    update_display();
}

static void sqrt_clicked(GtkWidget *btn, gpointer _){
    calc_sqrt(&state);
    update_display();
}

static void percent_clicked(GtkWidget *btn, gpointer _){
    calc_percent(&state);
    update_display();
}

static void reciprocal_clicked(GtkWidget *btn, gpointer _){
    calc_reciprocal(&state);
    update_display();
}
static gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer _){
    switch (event->keyval) {
        /* Digits */
        case GDK_KEY_0: case GDK_KEY_KP_0: calc_enter_digit(&state, 0); break;
        case GDK_KEY_1: case GDK_KEY_KP_1: calc_enter_digit(&state, 1); break;
        case GDK_KEY_2: case GDK_KEY_KP_2: calc_enter_digit(&state, 2); break;
        case GDK_KEY_3: case GDK_KEY_KP_3: calc_enter_digit(&state, 3); break;
        case GDK_KEY_4: case GDK_KEY_KP_4: calc_enter_digit(&state, 4); break;
        case GDK_KEY_5: case GDK_KEY_KP_5: calc_enter_digit(&state, 5); break;
        case GDK_KEY_6: case GDK_KEY_KP_6: calc_enter_digit(&state, 6); break;
        case GDK_KEY_7: case GDK_KEY_KP_7: calc_enter_digit(&state, 7); break;
        case GDK_KEY_8: case GDK_KEY_KP_8: calc_enter_digit(&state, 8); break;
        case GDK_KEY_9: case GDK_KEY_KP_9: calc_enter_digit(&state, 9); break;

        /* Decimal point */
        case GDK_KEY_period:
        case GDK_KEY_KP_Decimal:    calc_enter_dot(&state);              break;

        /* Operators */
        case GDK_KEY_plus:
        case GDK_KEY_KP_Add:        calc_press_op(&state, OP_ADD);       break;

        case GDK_KEY_minus:
        case GDK_KEY_KP_Subtract:   calc_press_op(&state, OP_SUB);       break;

        case GDK_KEY_asterisk:
        case GDK_KEY_KP_Multiply:   calc_press_op(&state, OP_MUL);       break;

        case GDK_KEY_slash:
        case GDK_KEY_KP_Divide:     calc_press_op(&state, OP_DIV);       break;

        /* Enter (Equals) */
        case GDK_KEY_equal:
        case GDK_KEY_Return:
        case GDK_KEY_KP_Enter:      calc_press_equals(&state);           break;

        /* Backspace — delete one digit */
        case GDK_KEY_BackSpace:     calc_backspace(&state);               break;

        /* Escape = C (clear all) */
        case GDK_KEY_Escape:        calc_clear_all(&state);              break;

        /* Delete = CE (clear entry) */
        case GDK_KEY_Delete:        calc_clear_entry(&state);            break;

        /* F9 = negate, like that useless feature from windows 7 */
        case GDK_KEY_F9:            calc_negate(&state);                 break;

        /* @ = sqrt  */
        case GDK_KEY_at:            calc_sqrt(&state);                   break;

        /* % */
        case GDK_KEY_percent:       calc_percent(&state);                break;

        /* r = 1/x (like that usefull feature from windows 7 */
        case GDK_KEY_r:
        case GDK_KEY_R:             calc_reciprocal(&state);             break;

        default:
            return FALSE; /* key not handled */
    }
    update_display();
    return TRUE; /* key was handled */
}
static void on_about_activate(GtkWidget *w, gpointer user_data){
    GtkWindow *parent = GTK_WINDOW(user_data);
    GtkWidget *dlg = gtk_message_dialog_new(parent,
        GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "Calculator\n Made by Microsoft. Inc.\n Copyright(c) 2009 Microsoft Corporation.\n All rights reserved");
    gtk_dialog_run(GTK_DIALOG(dlg));
    gtk_widget_destroy(dlg);
}

GtkWidget* create_main_window(GtkApplication *app){
    calc_init(&state);
    GtkStyleContext *ctx;
    GtkWidget *win = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(win), "Calculator");
    gtk_window_set_default_size(GTK_WINDOW(win), 205, 282);
    gtk_window_set_resizable(GTK_WINDOW(win), FALSE);

    GtkCssProvider *css = gtk_css_provider_new();
    gtk_css_provider_load_from_data(css,
        /* Window background — Win7 blue-grey gradient */
        /*"window { background-image: linear-gradient(to bottom, #f4f6fa, #f4f6fa, #f4f6fa, #dce6f4, #dce6f4, #c8d8ee); color: #1a1a1a; margin: 6px; }"*/
        "window { background-image: linear-gradient(to bottom, #edf4fb, #dae6f2, #dae6f2, #dae6f2, #dae6f2, #dae6f2); color: #1a1a1a; margin: 1.5px;  margin-right: 1.5px; padding: 6px; right:100%; top: 100%; font-size: 10px; }"
        "scrolledwindow.display-scroll scrollbar { min-width: 0; min-height: 0; opacity: 0;  }"\
        /* center the button grid */
        ".button-grid { margin: 3px; }"
        /* Display frame */
        ".calc-frame { "
        "  border: 0px solid #8e9cae; border-radius: 4px; padding: 1px; right: 100%; top: 100%;}"
        ".display-scroll { background-image: linear-gradient(to bottom, #d0ddf0, #ffffff); max-width: 188px; min-height: 48px; right: 100%;"
        "  border: 1px solid #8e9cae; border-radius: 2px;"
        "  font-size: 18px; font-weight: 600; color: #1a1a1a;  }"

        /* Standard calc buttons */
        "button.calc { background-image: linear-gradient(to bottom, #f4f7fd, #f4f7fd, #f4f7fd, #e0eef8, #e0eef8, #ffffff); right: 100%; top: 100%; margin: 1.5px; padding: 1px; margin-right: 1.5px;"
        "  border: 1px solid #8e9cae; border-radius: 3px;"
        "  font-size: 13px; padding: 1px;  }"
        "button.calc:hover { background-image: linear-gradient(to bottom, #e0ecfa, #c4d8f0); }"
        "button.calc:active { background-image: linear-gradient(to bottom, #c8d8ec, #dce8f8); }"

        /* Operator buttons (right column) */
        "button.op { background-image: linear-gradient(to bottom, #f4f7fd, #f4f7fd, #f4f7fd, #f4f7fd, #f4f7fd, #f4f7fd, #d5e2ed, #d5e2ed, #d5e2ed, #d5e2ed, #d5e2ed, #d5e2ed, #ffffff); margin-right: 1.5px; margin: 1.5px; right: 100%; top: 100%;"
        "  border: 1px solid #8e9cae; border-radius: 3px;"
        "  font-size: 13px; padding: 1px;  }"
        "button.op:hover { background-image: linear-gradient(to bottom, #e0ecfa, #c4d8f0); }"
        "button.op:active { background-image: linear-gradient(to bottom, #c8d8ec, #dce8f8); }"

        "button.eq { background-image: linear-gradient(to bottom, #f4f7fd, #f4f7fd, #f4f7fd, #f4f7fd, #f4f7fd, #f4f7fd, #f4f7fd, #f4f7fd, #f4f7fd, #f4f7fd, #f4f7fd, #f4f7fd, #d5e2ed, #d5e2ed, #d5e2ed, #d5e2ed, #d5e2ed, #d5e2ed, #d5e2ed, #d5e2ed, #d5e2ed, #d5e2ed, #d5e2ed, #d5e2ed, #ffffff); margin-right: 1.5px; margin: 1.5px; right: 100%; top: 100%;"
        "  border: 1px solid #8e9cae; border-radius: 3px;"
        "  font-size: 18px; font-weight: bold; padding: 1px;  }"
        "button.eq:hover { background-image: linear-gradient(to bottom, #e0ecfa, #c4d8f0); }"
        "button.eq:active { background-image: linear-gradient(to bottom, #c8d8ec, #dce8f8); }"

        "button.mem { background-image: linear-gradient(to bottom, #d5e1ef, #d5e1ef, #d5e1ef, #d5e1ef, #d5e1ef, #d5e1ef, #d5e1f2, #d5e1f2, #d5e1f2, #d5e1f2, #d5e1f2, #d5e1f2, #ffffff); margin: 1.5px; right: 100%; top: 100%; margin-right: 1.5px; "
        "  border: 1px solid #8e9cae; border-radius: 3px;"
        "  font-size: 11px; padding: 1px; color: #333; }"
        "button.mem:hover { background-image: linear-gradient(to bottom, #d5e1f2, #d5e1ef); }"
        , (gssize)-1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(css);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    gtk_container_add(GTK_CONTAINER(win), vbox);
    gtk_widget_set_margin_top(vbox, 0);
    gtk_widget_set_margin_bottom(vbox, 6);
    gtk_widget_set_margin_start(vbox, 6);
    gtk_widget_set_margin_end(vbox, 6);

    /* ── Menubar: View | Edit | Help ── */
    GtkWidget *menubar = gtk_menu_bar_new();

    /* View menu */
    GtkWidget *view_menu = gtk_menu_new();
    GtkWidget *view_item = gtk_menu_item_new_with_label("View");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(view_item), view_menu);
    GtkWidget *standard_item = gtk_menu_item_new_with_label("Standard");
    GtkWidget *scientific_item = gtk_menu_item_new_with_label("Scientific");
    gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), standard_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), scientific_item);

    /* Edit menu */
    GtkWidget *edit_menu = gtk_menu_new();
    GtkWidget *edit_item = gtk_menu_item_new_with_label("Edit");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit_item), edit_menu);
    GtkWidget *copy_item  = gtk_menu_item_new_with_label("Copy");
    GtkWidget *paste_item = gtk_menu_item_new_with_label("Paste");
    gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), copy_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(edit_menu), paste_item);

    /* Help menu */
    GtkWidget *help_menu = gtk_menu_new();
    GtkWidget *help_item = gtk_menu_item_new_with_label("Help");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(help_item), help_menu);
    GtkWidget *about_item = gtk_menu_item_new_with_label("About");
    g_signal_connect(about_item, "activate", G_CALLBACK(on_about_activate), win);
    gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), about_item);

    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), view_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), edit_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), help_item);
    gtk_box_pack_start(GTK_BOX(vbox), menubar, TRUE, TRUE, 0);

    /* ── Display ── */
    GtkWidget *frame = gtk_frame_new(NULL);
    ctx = gtk_widget_get_style_context(frame);
    gtk_style_context_add_class(ctx, "calc-frame");
    gtk_box_pack_start(GTK_BOX(vbox), frame, TRUE, TRUE, 0);

    GtkWidget *display_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    ctx = gtk_widget_get_style_context(display_box);
    gtk_style_context_add_class(ctx, "display");
    gtk_container_add(GTK_CONTAINER(frame), display_box);

    display_label = gtk_label_new("0");
    gtk_widget_set_halign(display_label, GTK_ALIGN_END);
    gtk_widget_set_valign(display_label, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_end(display_label, 8);
    gtk_label_set_xalign(GTK_LABEL(display_label), 1.0);

    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);  
    display_scroll = scroll;                                   
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
    GTK_POLICY_AUTOMATIC,
    GTK_POLICY_NEVER);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scroll),
    GTK_SHADOW_NONE);
    gtk_widget_set_size_request(scroll, 190, 48);
    gtk_widget_set_hexpand(scroll, TRUE);

    ctx = gtk_widget_get_style_context(scroll);               
    gtk_style_context_add_class(ctx, "display-scroll");

    gtk_container_add(GTK_CONTAINER(scroll), display_label);
    gtk_box_pack_start(GTK_BOX(display_box), scroll, TRUE, TRUE, 0);

    /* ── Button grid (5 columns) ── */
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 4);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 4);
    ctx = gtk_widget_get_style_context(grid);
    gtk_style_context_add_class(ctx, "button-grid");
    gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 0);

    /* Button width constants */
    const int BW = 34;  /* normal button width  */
    const int BH = 27;  /* normal button height */

    /* ── Row 0: Memory buttons — MC MR MS M+ M- ── */
    const char *mems[] = {"MC","MR","MS","M+","M\u2212"};
    for (int i = 0; i < 5; i++){
        GtkWidget *b = gtk_button_new_with_label(mems[i]);
        gtk_widget_set_size_request(b, BW, 27);
        ctx = gtk_widget_get_style_context(b);
        gtk_style_context_add_class(ctx, "mem");
        gtk_grid_attach(GTK_GRID(grid), b, i, 0, 1, 1);
    }

    /* ── Row 1: ← CE C ± √ ── */
    /* ← (backspace) */
    {
        GtkWidget *b = gtk_button_new_with_label("\u2190");
        gtk_widget_set_size_request(b, BW, BH);
        ctx = gtk_widget_get_style_context(b);
        gtk_style_context_add_class(ctx, "op");
        gtk_grid_attach(GTK_GRID(grid), b, 0, 1, 1, 1);
        g_signal_connect(b, "clicked", G_CALLBACK(clear_clicked), GINT_TO_POINTER(1));
    }
    /* CE */
    {
        GtkWidget *b = gtk_button_new_with_label("CE");
        gtk_widget_set_size_request(b, BW, BH);
        ctx = gtk_widget_get_style_context(b);
        gtk_style_context_add_class(ctx, "op");
        gtk_grid_attach(GTK_GRID(grid), b, 1, 1, 1, 1);
        g_signal_connect(b, "clicked", G_CALLBACK(clear_clicked), GINT_TO_POINTER(1));
    }
    /* C */
    {
        GtkWidget *b = gtk_button_new_with_label("C");
        gtk_widget_set_size_request(b, BW, BH);
        ctx = gtk_widget_get_style_context(b);
        gtk_style_context_add_class(ctx, "op");
        gtk_grid_attach(GTK_GRID(grid), b, 2, 1, 1, 1);
        g_signal_connect(b, "clicked", G_CALLBACK(clear_clicked), GINT_TO_POINTER(0));
    }
    /* ± */
    {
        GtkWidget *b = gtk_button_new_with_label("\u00b1");
        gtk_widget_set_size_request(b, BW, BH);
        ctx = gtk_widget_get_style_context(b);
        gtk_style_context_add_class(ctx, "op");
        gtk_grid_attach(GTK_GRID(grid), b, 3, 1, 1, 1);
        g_signal_connect(b, "clicked", G_CALLBACK(negate_clicked), NULL);
    }
    /* √ */
    {
        GtkWidget *b = gtk_button_new_with_label("\u221a");
        gtk_widget_set_size_request(b, BW, BH);
        ctx = gtk_widget_get_style_context(b);
        gtk_style_context_add_class(ctx, "op");
        gtk_grid_attach(GTK_GRID(grid), b, 4, 1, 1, 1);
        g_signal_connect(b, "clicked", G_CALLBACK(sqrt_clicked), NULL);
    }


    /* Digit + basic-op rows */
    typedef struct { const char *label; int col; int row; } Cell;
    Cell digit_cells[] = {
        {"7",0,2}, {"8",1,2}, {"9",2,2},
        {"4",0,3}, {"5",1,3}, {"6",2,3},
        {"1",0,4}, {"2",1,4}, {"3",2,4},
        {"0",0,5},
        {".",2,5},
    };
    int ncells = sizeof(digit_cells)/sizeof(digit_cells[0]);
    for (int i = 0; i < ncells; i++){
        const char *lab = digit_cells[i].label;
        GtkWidget *b = gtk_button_new_with_label(lab);
        /* 0 spans 2 columns */
        int span = (lab[0]=='0' && digit_cells[i].col==0) ? 2 : 1;
        gtk_widget_set_size_request(b, span==2 ? BW*2+4 : BW, BH);
        ctx = gtk_widget_get_style_context(b);
        gtk_style_context_add_class(ctx, "calc");
        gtk_grid_attach(GTK_GRID(grid), b, digit_cells[i].col, digit_cells[i].row, span, 1);
        if (lab[0] >= '0' && lab[0] <= '9'){
            g_signal_connect(b, "clicked", G_CALLBACK(digit_clicked), GINT_TO_POINTER(lab[0]-'0'));
        } else if (lab[0] == '.'){
            g_signal_connect(b, "clicked", G_CALLBACK(dot_clicked), NULL);
        }
    }

    /* Basic operators: / * - + in column 3, rows 2-5 */
    typedef struct { const char *label; Op op; int row; } OpCell;
    OpCell ops[] = {
        {"/", OP_DIV, 2},
        {"*", OP_MUL, 3},
        {"-", OP_SUB, 4},
        {"+", OP_ADD, 5},
    };
    for (int i = 0; i < 4; i++){
        GtkWidget *b = gtk_button_new_with_label(ops[i].label);
        gtk_widget_set_size_request(b, BW, BH);
        ctx = gtk_widget_get_style_context(b);
        gtk_style_context_add_class(ctx, "op");
        gtk_grid_attach(GTK_GRID(grid), b, 3, ops[i].row, 1, 1);
        g_signal_connect(b, "clicked", G_CALLBACK(op_clicked), GINT_TO_POINTER(ops[i].op));
    }

    /* Extra ops column 4: % (row2), 1/x (row3), then tall = (rows 4-5) */
    {
        GtkWidget *b = gtk_button_new_with_label("%");
        gtk_widget_set_size_request(b, BW, BH);
        ctx = gtk_widget_get_style_context(b);
        gtk_style_context_add_class(ctx, "op");
        gtk_grid_attach(GTK_GRID(grid), b, 4, 2, 1, 1);
        g_signal_connect(b, "clicked", G_CALLBACK(percent_clicked), NULL);
    }
    {
        GtkWidget *b = gtk_button_new_with_label("1/x");
        gtk_widget_set_size_request(b, BW, BH);
        ctx = gtk_widget_get_style_context(b);
        gtk_style_context_add_class(ctx, "op");
        gtk_grid_attach(GTK_GRID(grid), b, 4, 3, 1, 1);
        g_signal_connect(b, "clicked", G_CALLBACK(reciprocal_clicked), NULL);
    }

    /* Tall = button spanning rows 4-5 in column 4 */
    {
        GtkWidget *b = gtk_button_new_with_label("=");
        gtk_widget_set_size_request(b, BW, BH*2+4);
        ctx = gtk_widget_get_style_context(b);
        gtk_style_context_add_class(ctx, "eq");
        gtk_grid_attach(GTK_GRID(grid), b, 4, 4, 1, 2);
        g_signal_connect(b, "clicked", G_CALLBACK(eq_clicked), NULL);
    }
    g_signal_connect(win, "key-press-event", G_CALLBACK(on_key_press), NULL);
    gtk_widget_show_all(win);
    return win;
}
