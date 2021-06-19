//
// Created by theo on 6/19/2021.
//

#ifndef UMDWITHCLASS3_ERROR_H
#define UMDWITHCLASS3_ERROR_H


namespace error {
    /*
     *
     */
    enum Importance {
        UNIMPORTANT = 0,
        MILD = 1,
        IMPORTANT = 2,

        REQUIRES_USER_ACTION = 3,
        REQUIRES_APP_ACTION = 4,
    };

    /**
     * error class defines the way errors are send to host app
     * max size of name + message = 146 bytes (this wired number comes from the fact that the entire JsonDocument has 256 bytes)
     */
    class Error {

    public:
        String name;
        String message;
        bool createAlertDialog;
        Importance importance;

        /**
         * error class defines the way errors are send to host app
         * max size of name + message = 146 bytes (this wired number comes from the fact that the entire JsonDocument has 256 bytes)
         * sets createAlertDialog to true and importance to MILD (1)
         *
         * @param name name of the error
         * @param message details about the error
         */
        Error(String name, String message) : Error(name, message, true, error::Importance.MILD) {}

        /**
         * error class defines the way errors are send to host app
         * max size of name + message = 146 bytes (this wired number comes from the fact that the entire JsonDocument has 256 bytes)
         * sets createAlertDialog to true
         *
         * @param name name of the error
         * @param message details about the error
         * @param importance
         */
        Error(String name, String message, error::Importance importance) : Error(name, message, true, importance) {}

        /**
         * error class defines the way errors are send to host app
         * max size of name + message = 146 bytes (this wired number comes from the fact that the entire JsonDocument has 256 bytes)
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
        Error(String name, String message, bool createAlertDialog, error::Importance importance) : name(name),
                                                                                            message(message),
                                                                                            createAlertDialog(
                                                                                                    createAlertDialog),
                                                                                            importance(importance) {}

    };

}


#endif //UMDWITHCLASS3_ERROR_H
