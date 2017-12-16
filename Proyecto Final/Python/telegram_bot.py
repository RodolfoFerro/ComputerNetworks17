# ===============================================================
# Author: Rodolfo Ferro Pérez
# Email: ferro@cimat.mx
# Twitter: @FerroRodolfo
#
# Script: Telegram bot to turn on/off LED (IoT).
#
# ABOUT COPYING OR USING PARTIAL INFORMATION:
# This script was originally created by Rodolfo Ferro. Any
# explicit usage of this script or its contents is granted
# according to the license provided and its conditions.
# ===============================================================

from telegram import InlineKeyboardButton, InlineKeyboardMarkup, ParseMode
from telegram.ext import Updater, CommandHandler, CallbackQueryHandler
from turn_light import parser, turn_ligth
import logging

# You might need to add your tokens to this file...
# If this file is not appearing, create credentials.py,
# then create TELE_TOKEN equal to your Telegram bot token.
from credentials import *


# Config logger:
logging.basicConfig(
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
    level=logging.INFO)

logger = logging.getLogger(__name__)

# Set global vars:
LED = ['OFF', 'ON']
STATE = 0
URL = None


def start(bot, update):
    """
    Start function. Displayed whenever the /start command is called.
    This function sets the language of the bot.
    """

    # Create initial message:
    user = update.message.from_user.first_name
    message = "Hey {}, I'm a bot that can turn *ON*/*OFF* a LED.".format(user)
    bot.send_message(chat_id=update.message.chat_id,
                     text=message,
                     parse_mode=ParseMode.MARKDOWN)

    # Create keyboard:
    keyboard = [[InlineKeyboardButton("Turn it ON", callback_data='1'),
                 InlineKeyboardButton("Turn it OFF", callback_data='0')]]

    # Create reply markup:
    reply_markup = InlineKeyboardMarkup(keyboard)
    ask = "What do you want to do?"
    update.message.reply_text(ask, reply_markup=reply_markup)

    return


def button(bot, update):
    """
    This is a CallbackQueryHandler funtion for the start command.
    It can turn ON/OFF the LED using http requests.
    """

    query = update.callback_query
    state = int(query.data)

    # Set state:
    global STATE, URL
    if state != STATE:
        STATE = state
        turn_ligth(URL, LED[STATE])
        text = "Okay. The light was turned *{}*.".format(LED[STATE])
    else:
        text = "The light is already *{}*!".format(LED[STATE])

    # Return state on message:
    bot.edit_message_text(text=text,
                          chat_id=query.message.chat_id,
                          message_id=query.message.message_id,
                          parse_mode=ParseMode.MARKDOWN)

    # Send message to restart:
    restart = "Use /start to restart this bot."
    bot.send_message(chat_id=query.message.chat_id, text=restart)

    return


def help(bot, update):
    update.message.reply_text("Use /start to test this bot.")
    return


def error(bot, update, error):
    """Log Errors caused by Updates."""
    logger.warning('Update "%s" caused error "%s"', update, error)
    return


def main():
    # Create the Updater and pass it your bot's token.
    updater = Updater(TELE_TOKEN)

    # Parse url:
    global URL
    url, state = parser()
    URL = url

    # Register handlers:
    updater.dispatcher.add_handler(CommandHandler('start', start))
    updater.dispatcher.add_handler(CallbackQueryHandler(button))
    updater.dispatcher.add_handler(CommandHandler('help', help))
    updater.dispatcher.add_error_handler(error)

    # Start the Bot
    updater.start_polling()

    # Run the bot until the user presses Ctrl-C or the process receives SIGINT,
    # SIGTERM or SIGABRT
    updater.idle()


if __name__ == '__main__':
    main()
