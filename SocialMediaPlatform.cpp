#include "SocialMediaPlatform.h"
#include <iostream>
#include <vector>
using namespace std;

// User Class Implementation
User::User(const string &uname, const string &pwd, const string &email, const string &bio, bool isPublic)
    : username(uname), password(pwd), email(email), bio(bio), isPublic(isPublic) {}

string User::getUsername() { return username; }
string User::getEmail() { return email; }
string User::getBio() { return bio; }
bool User::isProfilePublic() { return isPublic; }
bool User::validatePassword(const string &pwd) { return password == pwd; }

void User::updateBio(const std::string &newBio) { bio = newBio; }
void User::updateEmail(const std::string &newEmail) { email = newEmail; }
void User::updatePassword(const std::string &newPassword) { password = newPassword; }
void User::updatePrivacy(bool newPrivacy) { isPublic = newPrivacy; }
void User::updateUsername(const std::string &newUsername) { username = newUsername; }

// UserManagement Class Implementation

User *UserManagement::validateUsername(const string &username)
{
    auto it = userCredentials.find(username);
    if (it != userCredentials.end())
    {
        return it->second.second; // Username exists, return the user object
    }
    return nullptr; // Username does not exist
}

bool UserManagement::isValidEmail(const string &email)
{
    // Find the position of '@' and '.' after '@'
    int atPos = -1;
    int dotPos = -1;

    // Loop through the string to find '@' and the last '.'
    for (int i = 0; i < email.length(); i++)
    {
        if (email[i] == '@')
        {
            atPos = i;
        }
        else if (email[i] == '.' && atPos != -1)
        {
            dotPos = i;
        }
    }

    // Basic checks: '@' should exist, '.' should come after '@', and they should not be at the start or end
    if (atPos > 0 && dotPos > atPos + 1 && dotPos < email.length() - 1)
    {
        return true;
    }
    return false;
}

User *UserManagement::signUp()
{
    string username, password, email, bio;
    bool isPublic;

    // Check if the username already exists
    while (true)
    {
        cout << "Enter username: ";
        cin >> username;
        if (validateUsername(username) == nullptr)
        {
            break; // Username is available
        }
        else
        {
            cout << "Username already taken! Please try again." << endl;
        }
    }

    cout << "Enter password: ";
    cin >> password;

    // Validate email
    while (true)
    {
        cout << "Enter email: ";
        cin >> email;
        if (isValidEmail(email))
        {
            break; // Valid email
        }
        else
        {
            cout << "Invalid email format! Please try again." << endl;
        }
    }

    cout << "Enter bio: ";
    cin.ignore();
    getline(cin, bio);

    // Error handling for public/private input
    while (true)
    {
        char choice;
        cout << "Do you want your profile to be public? (Y for Yes, N for No): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y' || choice == 'n' || choice == 'N')
        {
            isPublic = (choice == 'y' || choice == 'Y') ? 1 : 0; // Set isPublic to 1 for 'y', 0 for 'n'
            break;
        }
        else
        {
            cout << "Invalid input. Please enter 'Y' for Yes or 'N' for No." << endl;
        }
    }

    // Create a new user
    User *newUser = new User(username, password, email, bio, isPublic);
    userCredentials[username] = {password, newUser}; // Store username, password, and user object
    userProfiles.push_back(newUser);                 // Add user to linked list

    return newUser;
}

User *UserManagement::logIn(const string &username, const string &password)
{
    auto it = userCredentials.find(username);
    if (it != userCredentials.end() && it->second.first == password)
    {
        return it->second.second; // Return user if username and password match
    }
    return nullptr; // Invalid username or password
}

void UserManagement::editProfile(User *user)
{
    int choice;
    while (true)
    {
        cout << "Edit Profile Menu:" << endl;
        cout << "1. Change Username" << endl;
        cout << "2. Change Bio" << endl;
        cout << "3. Change Email" << endl;
        cout << "4. Change Password" << endl;
        cout << "5. Change Privacy Settings (Public/Private)" << endl;
        cout << "6. Go Back" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) // Change Username
        {
            string newUsername;
            while (true)
            {
                cout << "Enter new username: ";
                cin >> newUsername;

                if (validateUsername(newUsername) == nullptr) // Check if username is available
                {
                    string oldUsername = user->getUsername();

                    // Step 1: Update the username in the userCredentials map
                    userCredentials[newUsername] = userCredentials[oldUsername]; // Copy entry
                    userCredentials.erase(oldUsername);                          // Remove old entry

                    // Step 2: Update the User object with the new username
                    user->updateUsername(newUsername);

                    // Step 3: The user object is already in userProfiles (a list of pointers), so no need to update there,
                    //         but the username in the user object is now updated.

                    cout << "Username updated successfully!" << endl;
                    break;
                }
                else
                {
                    cout << "Username already taken! Please try again." << endl;
                }
            }
        }
        else if (choice == 2)
        {
            string newBio;
            cout << "Enter new bio: ";
            cin.ignore();
            getline(cin, newBio);
            user->updateBio(newBio); // Use updateBio method
            cout << "Bio updated successfully!" << endl;
        }
        else if (choice == 3)
        {
            string newEmail;
            while (true)
            {
                cout << "Enter new email: ";
                cin >> newEmail;
                if (isValidEmail(newEmail))
                {
                    user->updateEmail(newEmail);
                    cout << "Email updated successfully!" << endl;
                    break;
                }
                cout << "Invalid email format! Please try again." << endl;
            }
        }
        else if (choice == 4)
        {
            string newPassword, confirmPassword;
            cout << "Enter new password: ";
            cin >> newPassword;
            cout << "Confirm new password: ";
            cin >> confirmPassword;

            if (newPassword == confirmPassword)
            {
                user->updatePassword(newPassword);
                userCredentials[user->getUsername()].first = newPassword; // Update password in the map
                cout << "Password updated successfully!" << endl;
            }
            else
            {
                cout << "Passwords do not match. Try again." << endl;
            }
        }
        else if (choice == 5)
        {
            bool isPublic;
            while (true)
            {
                char choice;
                cout << "Do you want your profile to be public? (Y for Yes, N for No): ";
                cin >> choice;
                if (choice == 'y' || choice == 'Y' || choice == 'n' || choice == 'N')
                {
                    isPublic = (choice == 'y' || choice == 'Y');
                    user->updatePrivacy(isPublic); // Update privacy settings
                    cout << "Privacy settings updated successfully!" << endl;
                    break;
                }
                else
                {
                    cout << "Invalid input. Please enter 'Y' for Yes or 'N' for No." << endl;
                }
            }
        }
        else if (choice == 6)
        {
            break; // Go back to the main menu
        }
        else
        {
            cout << "Invalid option. Please try again." << endl;
        }
    }
}

void UserManagement::displayProfile(User *user)
{
    cout << "Username: " << user->getUsername() << endl;

    cout << "Email: " << user->getEmail() << endl;
    cout << "Bio: " << user->getBio() << endl;
    cout << "Profile Status: " << (user->isProfilePublic() ? "Public" : "Private") << endl;
}

User *UserManagement::findUserByUsername(const string &username)
{
    auto it = userCredentials.find(username);
    return (it != userCredentials.end()) ? it->second.second : nullptr; // Return user if found
}

void UserManagement::displayAllUsers()
{
    for (User *user : userProfiles)
    { // Iterate over linked list
        cout << user->getUsername() << endl;
    }
}

void Comment::addReply(User* replier, const string &replyContent)
{
        replies.emplace_back(replier, replyContent); // Add reply to replies list
}



void Comment::displayComment(int level) 
{
        // Indent based on the level of the comment for readability
        for (int i = 0; i < level; i++) cout << "  ";
        cout << getAuthor() << ": " << getContent() << endl;

        // Display all replies with increased indentation level
        for (auto &reply : replies) {
            reply.displayComment(level + 1);
        }
}

// PostManagement Class Implementation
void PostManagement::createPost(User *user, const string &content)
{
    userPosts[user].push_back(content);
     postComments[content] = {};
    cout << "post created successfully" << endl;
}

void PostManagement::viewUserPosts(User *user)
{
    auto it = userPosts.find(user);
    if (it != userPosts.end())
    {
        for (const string &post : it->second)
        {
            cout << post << endl;
        }
    }
    else
    {
        cout << "No posts found!" << endl;
    }
}

void PostManagement::addComment(User* user, const std::string& postContent, const std::string& commentContent) {
    // Displaying debug info
    std::cout << "Adding a new comment by user: " << user->getUsername() << std::endl;
    std::cout << "Post content: " << postContent << std::endl;
    std::cout << "Comment content: " << commentContent << std::endl;

    // Creating the comment
    Comment* newComment = new Comment(user, commentContent);
    postComments[postContent].emplace_back(newComment);  // Add Comment pointer

    // Confirm comment addition
    std::cout << "Comment added successfully to post: " << postContent << std::endl;
}

void PostManagement::addReplyToComment(User* user, const string &postContent, Comment* parentComment, const string &replyContent) {
    parentComment->addReply(user, replyContent);  // Use '->' since parentComment is now a pointer
}





void PostManagement::viewPostComments(const string &postContent) {
    auto it = postComments.find(postContent);
    if (it != postComments.end()) {
        cout << "Comments for post: " << postContent << endl;
        for (auto &comment : it->second) {
            comment->displayComment();  // Use '->' instead of '.'
        }
    } else {
        cout << "No comments found for this post." << endl;
    }
}




void PostManagement::viewFriendsPosts(User *user, const map<User *, list<User *>> &friends)
{
    auto it = friends.find(user);
    if (it != friends.end())
    {
        for (User *friendUser : it->second)
        {
            cout << "Posts by " << friendUser->getUsername() << ":" << endl;
            viewUserPosts(friendUser);
        }
    }
    else
    {
        cout << "No friends found!" << endl;
    }
}

void PostManagement::viewPublicPosts(const map<User *, list<string>> &userPosts, User *currentUser)
{
    for (const auto &pair : userPosts)
    {
        User *user = pair.first;
        if (user != currentUser && user->isProfilePublic())
        {
            cout << "Posts by " << user->getUsername() << " (Public Profile):" << endl;
            for (const string &post : pair.second)
            {
                cout << post << endl;
            }
        }
    }
}

vector<string> PostManagement::getAllPosts()
{
    vector<string> posts;
    for (const auto &pair : userPosts)
    {
        for (const string &post : pair.second)
        {
            posts.push_back(post);
        }
    }
    return posts; // Return all posts in a vector
}

// FriendSystem Class Implementation
void FriendSystem::addFriend(User *user, User *friendUser)
{
    friends[user].push_back(friendUser);
}

void FriendSystem::viewFriends(User *user)
{
    auto it = friends.find(user);
    if (it != friends.end())
    {
        for (User *friendUser : it->second)
        {
            cout << friendUser->getUsername() << endl;
        }
    }
    else
    {
        cout << "No friends found!" << endl;
    }
}

map<User *, list<User *>> FriendSystem::getFriendsList()
{
    return friends;
}

// MessagingSystem Class Implementation
void MessagingSystem::sendMessage(User *fromUser, User *toUser, const string &message)
{
    userMessages[toUser].push(fromUser->getUsername() + ": " + message);
}

void MessagingSystem::viewMessages(User *user)
{
    auto it = userMessages.find(user);
    if (it != userMessages.end())
    {
        while (!it->second.empty())
        {
            cout << it->second.front() << endl;
            it->second.pop();
        }
    }
    else
    {
        cout << "No messages found!" << endl;
    }
}

// Function to display the header
void displayHeader()
{
    cout << "************************************************************" << endl;
    cout << "*                    WELCOME TO OUR                        *" << endl;
    cout << "*                    College Connect                       *" << endl;
    cout << "*                                                          *" << endl;
    cout << "* Submitted to:                             Programmed by: *" << endl;
    cout << "* Sherry Garg                                        Rishu *" << endl;
    cout << "* Sangeeta Mittal                             Swayam Gupta *" << endl;
    cout << "*                                             Maanya Gupta *" << endl;
    cout << "*                                         Shambhavi Mishra *" << endl;
    cout << "*                                                          *" << endl;
    cout << "************************************************************" << "\n"
         << endl;
}
void showMenu()
{
    displayHeader();
    cout << "1. Sign Up" << endl;
    cout << "2. Log In" << endl;
    cout << "3. Exit" << endl;
}

void showUserMenu()
{
    displayHeader();
    cout << "1. View Profile" << endl;
    cout << "2. Edit Profile" << endl;
    cout << "3. Create Post" << endl;
    cout << "4. View My Posts" << endl;
    cout << "5. View Friends' Posts" << endl;
    cout << "6. View Public Posts" << endl;
    cout << "7. View all users" << endl;
    cout << "8. Add Friend" << endl;
    cout << "9. Send Message" << endl;
    cout << "10. View Messages" << endl;
    cout << "11. Log Out" << endl;
}

int main()
{
    UserManagement userManagement;
    PostManagement postManagement;
    FriendSystem friendSystem;
    MessagingSystem messagingSystem;

    User *currentUser = nullptr;

    while (true)
    {
        showMenu();
        int choice;
        cin >> choice;

        if (choice == 1)
        {
            currentUser = userManagement.signUp();
            if (currentUser)
            {
                cout << "Sign Up successful! You can now log in." << endl;
            }
        }

        else if (choice == 2)
        {
            string username, password;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;

            currentUser = userManagement.logIn(username, password);
            if (currentUser)
            {
                cout << "Log In successful!" << endl;
                while (true)
                {
                    showUserMenu();
                    int userChoice;
                    cin >> userChoice;

                    if (userChoice == 1)
                    {
                        userManagement.displayProfile(currentUser);
                        sleep(1);
                    }
                    else if (userChoice == 2)
                    {
                        userManagement.editProfile(currentUser); // New edit profile functionality
                        sleep(1);
                    }
                    else if (userChoice == 3)
                    {
                        string content;
                        cout << "Enter your post: ";
                        cin.ignore();
                        getline(cin, content);
                        postManagement.createPost(currentUser, content);
                        sleep(1);
                    }
                    else if (userChoice == 4)
                    {
                        postManagement.viewUserPosts(currentUser);
                        sleep(1);
                    }
                    else if (userChoice == 5)
                    {
                        postManagement.viewFriendsPosts(currentUser, friendSystem.getFriendsList());
                        sleep(1);
                    }
                    else if (userChoice == 6)
                    {
                        postManagement.viewPublicPosts(postManagement.userPosts, currentUser);
                        sleep(1);
                    }
                    else if (userChoice == 7)
                    {
                        userManagement.displayAllUsers();
                        sleep(1);
                    }
                    else if (userChoice == 8)
                    {
                        userManagement.displayAllUsers();
                        sleep(1);
                        string friendUsername;
                        cout << "Enter friend's username: ";
                        cin >> friendUsername;
                        User *friendUser = userManagement.findUserByUsername(friendUsername);
                        if (friendUser)
                        {
                            friendSystem.addFriend(currentUser, friendUser);
                            cout << "Friend added!" << endl;
                            sleep(1);
                        }
                        else
                        {
                            cout << "User not found!" << endl;
                            sleep(1);
                        }
                    }
                    else if (userChoice == 9)
                    {
                        friendSystem.viewFriends(currentUser);
                        string recipientUsername, message;
                        cout << "Enter recipient's username: ";
                        cin >> recipientUsername;
                        User *recipient = userManagement.findUserByUsername(recipientUsername);
                        if (recipient)
                        {
                            cout << "Enter your message: ";
                            cin.ignore();
                            getline(cin, message);
                            messagingSystem.sendMessage(currentUser, recipient, message);
                            sleep(1);
                        }
                        else
                        {
                            cout << "User not found!" << endl;
                            sleep(1);
                        }
                    }
                    else if (userChoice == 10)
                    {
                        messagingSystem.viewMessages(currentUser);
                        sleep(1);
                    }
                    else if (userChoice == 11)
                    {
                        currentUser = nullptr; // Log out
                        break;
                    }




else if (userChoice == 12) {  // Add Comment to Post
    userManagement.displayAllUsers();
    string postContent, commentContent;
    cout << "Enter the content of the post you want to comment on: ";
    cin.ignore();
    getline(cin, postContent);
    cout << "Enter your comment: ";
    getline(cin, commentContent);
    postManagement.addComment(currentUser, postContent, commentContent);
} 

                }
            }
            else
            {
                cout << "Invalid username or password!" << endl;
            }
        }
        else if (choice == 3)
        {
            break; // Exit
        }
    }

    return 0;
}


