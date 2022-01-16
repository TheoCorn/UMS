//
// Created by theo on 6/19/2021.
//

#ifndef UMDWITHCLASS3_ERROR_H
#define UMDWITHCLASS3_ERROR_H

constexpr const char* ERROR__MSG_FAILED_TO_PARSE_JSON_NAME = "failed to parse json configuration";
constexpr const char* ERROR_MSG__SET_SENSOR_CONFIG_JSON_FAILURE_MESSAGE = "unable to set sensor config json";
constexpr const char* ERROR_MSG__INVALID_FILE = "invalid file";
constexpr const char* ERROR_MSG__INVALID_ARGUMENT = "invalid argument";
constexpr const char* ERROR_MSG__INVALID_FEATURE = "invalid feature index";
constexpr const char*ERROR_MSG__INVALID_XSETTING = "invalid Xsetting";
constexpr const char*ERROR_MSG__FAILED_TO_IDENTIFY_COMMAND = "failed to identify command";
constexpr const char* ERROR_MSG__FAILED_TO_OPEN_FILE = "could not open file";
constexpr const char* ERROR_MSG__FAILED_TO_SAVE = "failed to save";


namespace error {
    /**
     * sets the appearance of an error there is no guarantee the request will be accepted unless you use Importance::REQUIRES_USER_ACTION
     * (excluding Appearance::ERROR_TERMINAL)
     *
     * @see Importance
     */
    enum class Appearance: unsigned int {
        ///doesnt create any ui depending on app settings will log to error terminal
        NONE            = 0,

        ///prompts the app to create an alert dialog
        ALERT_DIALOG    = 1,

        ///prompts the app to create a snack bar dialog
        SNACK_BAR       = 2,

        /**
         * guarantees the error will be logged in error terminal
         * should be used for debugging instead of NONE
         */
        ERROR_TERMINAL  = 3
    };

    /**
     * defines the importance of an error
     */
    enum class Importance: unsigned int {
        UNIMPORTANT = 0,
        MILD = 1,
        IMPORTANT = 2,

        //todo rename to HOT
        /**
         * this is a special importance that guarantees the user will be informed about the error via the Appearance requested
         */
        REQUIRES_USER_ACTION = 3,
    };

    enum class BackgroundAppActions: unsigned int{
        NONE    = 0,
        RESEND  = 1,
    };

    /**
     * Error class defines the way errors are send to host app
     * max size of name + message = 158 bytes (this wired number comes from the fact that the entire JsonDocument has 256 bytes)
     * @class Error
     */
    class Error {

    public:
        const char* name;
        const char* message;
        unsigned int appearance;
        unsigned int importance;
        unsigned int backgroundAction;

        /**
         * Error class defines the way errors are send to host app
         * max size of name + message = 158 bytes (this wired number comes from the fact that the entire JsonDocument has 256 bytes)
         * sets createAlertDialog to true and importance to MILD (1)
         *
         * @param name name of the error
         * @param message details about the error
         */
        Error(const char* name, const char* message) : Error(name, message, error::Appearance::SNACK_BAR, error::Importance::MILD, BackgroundAppActions::NONE) {}

        /**
         * Error class defines the way errors are send to host app
         * max size of name + message = 158 bytes (this wired number comes from the fact that the entire JsonDocument has 256 bytes)
         * prompts the creation of a snackbar dialog
         *
         * @param name name of the error
         * @param message details about the error
         * @param importance
         */
        Error(const char* name, const char* message, error::Importance importance) : Error(name, message, error::Appearance::SNACK_BAR, importance, BackgroundAppActions::NONE) {}

        /**
         * Error defines the way errors are send to host app
         * max size of name + message = 158 bytes (this wired number comes from the fact that the entire JsonDocument has 256 bytes)
         * sets createAlertDialog to true
         *
         * @param name name of the error
         *
         * @param message details about the error
         *
         * @param createAlertDialog whether an alert dialog or other type of view component will be shown
         * the specific type depends on platform and user preferences
         * if you want to be sure a dialog will be shown use Importance.REQUIRES_USER_ACTION
         *
         * @param importance
         */
        Error(const char* name, const char* message, error::Appearance ui, error::Importance importance, error::BackgroundAppActions backgroundAction) : name(name),
                                                                                            message(message),
                                                                                            appearance(static_cast<unsigned int>(ui)),
                                                                                            importance(static_cast<unsigned int>(importance)),
                                                                                            backgroundAction(static_cast<unsigned int>(backgroundAction)){}

    };

}


#endif //UMDWITHCLASS3_ERROR_H
