#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

#include "../common/texture.h"
#include "../common/wrapper.h"
#include "median.h"


#define PLUGIN_MEDIAN_FILTER_CONFIG "median-filter-cfg"


typedef struct {
	size_t width, height;
} median_config_t;


static void query(void)
{
	static GimpParamDef args[] = {
		{ GIMP_PDB_INT32, "run-mode", "Run mode" },
		{ GIMP_PDB_IMAGE, "image", "Input image" },
		{ GIMP_PDB_DRAWABLE, "drawable", "Input drawable" }
	};

	gimp_install_procedure("median",
	                       "Median filter",
	                       "Performs median filtering",
	                       "Łukasz Hanuszczak",
	                       "Łukasz Hanuszczak",
	                       "2015",
	                       "<Image>/Filters/Median...",
	                       "RGB*, GRAY*",
	                       GIMP_PLUGIN,
	                       G_N_ELEMENTS(args), 0,
	                       args, NULL);
}


struct progress_context {
	uint8_t chan;
	uint8_t bpp;
};


static void filtering_progress(void *data, float progress)
{
	struct progress_context *ctx = (struct progress_context *)data;
	gimp_progress_update((progress + (float)ctx->chan)/(float)ctx->bpp);
}


static bool show_median_dialog(median_config_t *cfg)
{
	gimp_ui_init("median", false);

	GtkWidget *dialog = gimp_dialog_new("Median filter", "median",
	                                    NULL, 0,
	                                    gimp_standard_help_func,
	                                    PLUGIN_MEDIAN_FILTER_CONFIG,
	                                    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
	                                    GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);

	/* Spinner for `width` value. */
	GtkWidget *width_spinner = gtk_spin_button_new_with_range(0, 100, 1);
	GtkAdjustment *width_spinner_adj =
		gtk_spin_button_get_adjustment(GTK_SPIN_BUTTON(width_spinner));
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(width_spinner), cfg->width);
	g_signal_connect(width_spinner_adj, "value_changed",
	                 G_CALLBACK(gimp_int_adjustment_update), &cfg->width);
	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), width_spinner);

	/* Spinner for `height` value. */
	GtkWidget *height_spinner = gtk_spin_button_new_with_range(0, 100, 1);
	GtkAdjustment *height_spinner_adj =
		gtk_spin_button_get_adjustment(GTK_SPIN_BUTTON(height_spinner));
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(height_spinner), cfg->height);
	g_signal_connect(height_spinner_adj, "value_changed",
	                 G_CALLBACK(gimp_int_adjustment_update), &cfg->height);
	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), height_spinner);

	gtk_widget_show_all(dialog);

	bool result = gimp_dialog_run(GIMP_DIALOG(dialog)) == GTK_RESPONSE_OK;
	gtk_widget_destroy(dialog);
	return result;
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

	/* Run the median algorithm. */
	gimp_progress_init("Applying median filter...");

	GimpDrawable *drawable = gimp_drawable_get(args[2].data.d_drawable);
	uint8_t bpp = gimp_drawable_bpp(drawable->drawable_id);

	texture_t tex_in;
	gimp_drawable_texture_init(&tex_in, drawable);

	texture_t tex_out;
	texture_alloc(&tex_out, tex_in.width, tex_in.height, tex_in.stride);

	median_config_t cfg = {
		.width = 10,
		.height = 10
	};

	switch (args[0].data.d_int32) {
	case GIMP_RUN_INTERACTIVE:
		gimp_get_data(PLUGIN_MEDIAN_FILTER_CONFIG, &cfg);
		if (!show_median_dialog(&cfg)) {
			return;
		}
		gimp_set_data(PLUGIN_MEDIAN_FILTER_CONFIG,
		              &cfg, sizeof(median_config_t));
		break;
	case GIMP_RUN_WITH_LAST_VALS:
		gimp_get_data(PLUGIN_MEDIAN_FILTER_CONFIG, &cfg);
		break;
	case GIMP_RUN_NONINTERACTIVE:
		/* Not supported. */
		return;
	}

	/* Apply median algorithm for each channel. */
	for (uint8_t chan = 0; chan < bpp; chan++) {
		struct progress_context ctx = { .chan = chan, .bpp = bpp };
		median_filter(&tex_in, &tex_out, cfg.width, cfg.height,
		              &ctx, filtering_progress);
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
