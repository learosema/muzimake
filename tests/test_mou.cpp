/*
        mouse.c - The following program demonstrates how
        to use the mouse interrupt (0x33) with DOS/4GW.

        Compile and link: wcl386 -l=dos4g mouse
*/
#include <stdio.h>
#include <dos.h>
#include <i86.h>


#include <mouse.h>
#include <dpmiutil.h>
#include <textmode.h>

/* Data touched at mouse callback time -- they are
   in a structure to simplify calculating the size
   of the region to lock.
*/
struct callback_data {
    int             right_button;
    int             mouse_event;
    unsigned short  mouse_code;
    unsigned short  mouse_bx;
    unsigned short  mouse_cx;
    unsigned short  mouse_dx;
    signed short    mouse_si;
    signed short    mouse_di;
} cbd = { 0 };

#pragma off( check_stack )
void _loadds far click_handler( int max, int mbx,
                                int mcx, int mdx,
                                int msi, int mdi )
{
#pragma aux click_handler __parm [__eax] [__ebx] [__ecx] \
                               [__edx] [__esi] [__edi]
    cbd.mouse_event = 1;

    cbd.mouse_code = (unsigned short)max;
    cbd.mouse_bx   = (unsigned short)mbx;
    cbd.mouse_cx   = (unsigned short)mcx;
    cbd.mouse_dx   = (unsigned short)mdx;
    cbd.mouse_si   = (signed short)msi;
    cbd.mouse_di   = (signed short)mdi;
    if( cbd.mouse_code & 8 )
        cbd.right_button = 1;
}

/* Dummy function so we can calculate size of
  code to lock (cbc_end - click_handler).
*/
void cbc_end( void )
{
}
#pragma on( check_stack )

void main (void)
{
    int installed = 0;

		textmode_setmode(3);

    /* check for mouse driver */


    if( installed = mouse_init() )
        printf( "Mouse installed...\n" );
    else
        printf( "Mouse NOT installed...\n" );

    if( installed ) {
        /* lock callback code and data (essential under VMM!)
           note that click_handler, although it does a far
           return and is installed using a full 48-bit pointer,
           is really linked into the flat model code segment
           -- so we can use a regular (near) pointer in the
           lock_region() call.
        */
        if( (dpmi_lock_linear_region( &cbd, sizeof( cbd ) ) != 0) ||
            (dpmi_lock_linear_region( (void near *)click_handler,
               (char *)cbc_end - (char near *)click_handler ) != 0) )
        {
            printf( "locks failed\n" );
        } else {
            /* show mouse cursor */
						mouse_show();
            /* install click watcher */

						mouse_set_eventhandler((far_function_ptr_t)click_handler,
						0x0002 + 0x0008
						);

            while( !cbd.right_button ) {
                if( cbd.mouse_event ) {
                    printf( "Ev %04hxh  BX %hu  CX %hu  DX %hu  "
                            "SI %hd  DI %hd\n",
                            cbd.mouse_code, cbd.mouse_bx,
                            cbd.mouse_cx, cbd.mouse_dx,
                            cbd.mouse_si, cbd.mouse_di );
                    cbd.mouse_event = 0;
                }
            }
        }
    }
		mouse_hide();

    /* check installation again (to clear watcher) */
		mouse_init();
    printf( "Press Enter key to return\n" );
    getc( stdin );
		textmode_setmode(3);
}
