#include <stdbool.h>

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

#include "../common/texture.h"
#include "../common/wrapper.h"


#define PLUGIN_RADIAL_DISTORTION_CONFIG "radial-dist-cfg"


enum distortion_mode {
	DISTORTION_MODE_MULT,
	DISTORTION_MODE_DIV
};

enum distortion_sampler {
	DISTORTION_SAMPLER_NN,
	DISTORTION_SAMPLER_BILINEAR
};

typedef struct {
	float a, b;
	enum distortion_sampler sampler;
	enum distortion_mode mode;
} distortion_config_t;


static void texture_scale(const texture_t *in, texture_t *out,
                          sampler_t sampler)
{
	for (size_t y = 0; y < out->height; y++) {
		for (size_t x = 0; x < out->width; x++) {
			float u = (2.0f * (float)x + 1.0f) / (2.0f * (float)out->width);
			float v = (2.0f * (float)y + 1.0f) / (2.0f * (float)out->height);
			texture_set_pixel(out, x, y, sampler(in, u, v));
		}
	}
}


static void texture_distortion_correction(const texture_t *in, texture_t *out,
                                          const distortion_config_t *cfg)
{
	sampler_t sampler;
	switch (cfg->sampler) {
	case DISTORTION_SAMPLER_NN:
		sampler = texture_sample_nn;
		break;
	case DISTORTION_SAMPLER_BILINEAR:
		sampler = texture_sample_bilinear;
		break;
	}

	for (size_t y = 0; y < out->height; y++) {
		for (size_t x = 0; x < out->width; x++) {
			float u = (2.0f * (float)x + 1.0f) / (2.0f * (float)out->width);
			float v = (2.0f * (float)y + 1.0f) / (2.0f * (float)out->height);

			u = 2.0f * u - 1.0f;
			v = 2.0f * v - 1.0f;

			float r = hypot(u, v);
			float f = 1.0f + cfg->a * r * r + cfg->b * r * r * r * r;

			switch (cfg->mode) {
			case DISTORTION_MODE_MULT:
				u = u * f;
				v = v * f;
				break;
			case DISTORTION_MODE_DIV:
				u = u / f;
				v = v / f;
				break;
			}

			u = (u + 1.0f) / 2.0f;
			v = (v + 1.0f) / 2.0f;
			texture_set_pixel(out, x, y, sampler(in, u, v));
		}
	}
}


static void set_mode_mult(GtkToggleButton *button, gpointer data)
{
	(void)button;
	*(enum distortion_mode *)data = DISTORTION_MODE_MULT; 
}


static void set_mode_div(GtkToggleButton *button, gpointer data)
{
	(void)button;
	*(enum distortion_mode *)data = DISTORTION_MODE_DIV;
}


static void set_sampler_nn(GtkToggleButton *button, gpointer data)
{
	(void)button;
	*(enum distortion_sampler *)data = DISTORTION_SAMPLER_NN;
}


static void set_sampler_bilinear(GtkToggleButton *button, gpointer data)
{
	(void)button;
	*(enum distortion_sampler *)data = DISTORTION_SAMPLER_BILINEAR;
}


static bool show_distortion_dialog(distortion_config_t *cfg)
{
	gimp_ui_init("distortion", false);

	GtkWidget *dialog = gimp_dialog_new("Radial distortion", "distortion",
	                                    NULL, 0,
	                                    gimp_standard_help_func,
	                                    PLUGIN_RADIAL_DISTORTION_CONFIG,
	                                    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
	                                    GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);

	/* Spinner for 'a' value. */
	GtkWidget *a_spinner = gtk_spin_button_new_with_range(0.0, 10.0, 0.1);
	GtkAdjustment *a_spinner_adj =
		gtk_spin_button_get_adjustment(GTK_SPIN_BUTTON(a_spinner));
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(a_spinner), cfg->a);
	g_signal_connect(a_spinner_adj, "value_changed",
	                 G_CALLBACK(gimp_float_adjustment_update), &cfg->a);
	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), a_spinner);

	/* Spinner for 'b' value. */
	GtkWidget *b_spinner = gtk_spin_button_new_with_range(0.0, 10.0, 0.1);
	GtkAdjustment *b_spinner_adj =
		gtk_spin_button_get_adjustment(GTK_SPIN_BUTTON(b_spinner));
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(b_spinner), cfg->b);
	g_signal_connect(b_spinner_adj, "value_changed",
	                 G_CALLBACK(gimp_float_adjustment_update), &cfg->b);
	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), b_spinner);

	/* Mode radio buttons. */
	GtkWidget *mode_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
	GtkWidget *mode_mult_radio =
		gtk_radio_button_new_with_label(NULL, "multiply");
	GtkWidget *mode_div_radio =
		gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(mode_mult_radio),
		                                            "divide");
	gtk_box_pack_start(GTK_BOX(mode_box), mode_mult_radio, true, true, 2);
	gtk_box_pack_start(GTK_BOX(mode_box), mode_div_radio, true, true, 2);
	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), mode_box);
	g_signal_connect(mode_mult_radio, "toggled",
	                 G_CALLBACK(set_mode_mult), &cfg->mode);
	g_signal_connect(mode_div_radio, "toggled",
	                 G_CALLBACK(set_mode_div), &cfg->mode);

	/* Sampler radio buttons. */
	GtkWidget *sampler_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
	GtkWidget *sampler_bilinear_radio =
		gtk_radio_button_new_with_label(NULL, "bilinear");
	GtkWidget *sampler_nn_radio =
		gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(sampler_bilinear_radio),
		                                            "nearest-neighbour");
	gtk_box_pack_start(GTK_BOX(sampler_box), sampler_bilinear_radio,
	                   true, true, 2);
	gtk_box_pack_start(GTK_BOX(sampler_box), sampler_nn_radio,
	                   true, true, 2);
	g_signal_connect(sampler_bilinear_radio, "toggled",
	                 G_CALLBACK(set_sampler_bilinear), &cfg->sampler);
	g_signal_connect(sampler_nn_radio, "toggled",
	                 G_CALLBACK(set_sampler_nn), &cfg->sampler);
	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), sampler_box);

	gtk_widget_show_all(dialog);

	bool result = gimp_dialog_run(GIMP_DIALOG(dialog)) == GTK_RESPONSE_OK;
	gtk_widget_destroy(dialog);
	return result;
}


static void query(void)
{
	static GimpParamDef args[] = {
		{ GIMP_PDB_INT32, "run-mode", "Run mode" },
		{ GIMP_PDB_IMAGE, "image", "Input image" },
		{ GIMP_PDB_DRAWABLE, "drawable", "Input drawable" }
	};

	gimp_install_procedure("distortion",
	                       "Distortion filter",
	                       "Applies radial distortion filter",
	                       "Łukasz Hanuszczak",
	                       "Łukasz Hanuszczak",
	                       "2015",
	                       "<Image>/Filters/Distortion...",
	                       "RGB*, GRAY*",
	                       GIMP_PLUGIN,
	                       G_N_ELEMENTS(args), 0,
	                       args, NULL);
}


static void run(const gchar *name,
                gint argc, const GimpParam *args,
                gint *resultc, GimpParam **results)
{
	(void)name;
	(void)argc;

	/* Setup result (initially as success and change to error if required). */
	static GimpParam values[1];
	values[0].type = GIMP_PDB_STATUS;
	values[0].data.d_status = GIMP_PDB_SUCCESS;

	*resultc = 1;
	results[0] = values;

	/* Run the distortion algorithm. */
	GimpDrawable *drawable = gimp_drawable_get(args[2].data.d_drawable);
	uint8_t bpp = gimp_drawable_bpp(drawable->drawable_id);

	texture_t tex_in;
	gimp_drawable_texture_init(&tex_in, drawable);

	texture_t tex_out;
	texture_alloc(&tex_out, tex_in.width, tex_in.height, tex_in.stride);

	distortion_config_t cfg = {
		.a = 0.5f, .b = 0.5f,
		.sampler = DISTORTION_SAMPLER_BILINEAR,
		.mode = DISTORTION_MODE_MULT
	};

	switch (args[0].data.d_int32) {
	case GIMP_RUN_INTERACTIVE:
		gimp_get_data(PLUGIN_RADIAL_DISTORTION_CONFIG, &cfg);
		if (!show_distortion_dialog(&cfg)) {
			return;
		}
		gimp_set_data(PLUGIN_RADIAL_DISTORTION_CONFIG,
		              &cfg, sizeof(distortion_config_t));
		break;
	case GIMP_RUN_WITH_LAST_VALS:
		gimp_get_data(PLUGIN_RADIAL_DISTORTION_CONFIG, &cfg);
		break;
	case GIMP_RUN_NONINTERACTIVE:
		/* Not supported. */
		return;
	}

	/* Apply distortion algorithm for each channel. */
	for (uint8_t chan = 0; chan < bpp; chan++) {
		texture_distortion_correction(&tex_in, &tex_out, &cfg);
		tex_in.data++;
		tex_out.data++;
	}
	tex_in.data -= bpp;
	tex_out.data -= bpp;

	gimp_drawable_texture_blit(&tex_out, drawable);

	texture_dealloc(&tex_out);
	gimp_drawable_texture_free(&tex_in);

	gimp_drawable_detach(drawable);
	gimp_displays_flush();
}


GimpPlugInInfo PLUG_IN_INFO = {
	NULL, NULL,
	query, run
};


MAIN()
