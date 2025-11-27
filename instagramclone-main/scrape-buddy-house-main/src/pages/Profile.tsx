import { useState } from 'react';
import { Navigation } from '@/components/Navigation';
import { Avatar, AvatarImage, AvatarFallback } from '@/components/ui/avatar';
import { Button } from '@/components/ui/button';
import { Separator } from '@/components/ui/separator';
import { FollowersDialog } from '@/components/FollowersDialog';
import { EditPostDialog } from '@/components/EditPostDialog';
import { DeletePostDialog } from '@/components/DeletePostDialog';
import { EditProfileDialog } from '@/components/EditProfileDialog';
import { CreatePostDialog } from '@/components/CreatePostDialog';
import { CreateStoryDialog } from '@/components/CreateStoryDialog';
import { Grid, Bookmark, Tag, Edit2, Trash2, Menu, Share2, Repeat2 } from 'lucide-react';
import { BottomNavigation } from '@/components/BottomNavigation';
import {
  DropdownMenu,
  DropdownMenuContent,
  DropdownMenuItem,
  DropdownMenuTrigger,
} from '@/components/ui/dropdown-menu';
import { useParams, Link } from 'react-router-dom';
import { getCurrentUser, getUsers } from '@/lib/localStorage';
import { usePosts, useCurrentUser } from '@/hooks/useLocalStorage';
import { toast } from 'sonner';
import type { Post } from '@/data/mockData';

const Profile = () => {
  const { username } = useParams();
  const { currentUser, updateProfile } = useCurrentUser();
  const { posts, updatePost, removePost, addPost } = usePosts();
  const [activeTab, setActiveTab] = useState<'posts' | 'reposts' | 'tagged'>('posts');
  
  const currentUsername = getCurrentUser();
  const allUsers = getUsers();
  const profileUser = allUsers.find(u => u.username === username);
  
  const userPosts = posts.filter((p) => p.username === username);
  const user = currentUser?.username === username ? currentUser : profileUser || (userPosts[0] ? {
    id: userPosts[0].userId,
    username: userPosts[0].username,
    fullName: userPosts[0].username,
    avatar: userPosts[0].userAvatar,
    bio: '',
    followers: 0,
    following: 0,
    posts: userPosts.length,
  } : undefined);
  
  const [followersOpen, setFollowersOpen] = useState(false);
  const [followingOpen, setFollowingOpen] = useState(false);
  const [editingPost, setEditingPost] = useState<Post | null>(null);
  const [deletingPost, setDeletingPost] = useState<Post | null>(null);
  const [hoveredPostId, setHoveredPostId] = useState<string | null>(null);
  const [editProfileOpen, setEditProfileOpen] = useState(false);
  const [createPostOpen, setCreatePostOpen] = useState(false);
  const [createStoryOpen, setCreateStoryOpen] = useState(false);
  
  const isOwnProfile = currentUser?.username === username;
  
  // Follow state
  const followData = JSON.parse(localStorage.getItem('followData') || '{}');
  const userFollowing = followData[currentUsername]?.following || [];
  const [isFollowing, setIsFollowing] = useState(userFollowing.includes(username));

  // Get reposts and tagged posts
  const repostIds = JSON.parse(localStorage.getItem('reposts') || '{}')[username] || [];
  const reposts = posts.filter(post => repostIds.includes(post.id));
  
  const taggedPostIds = JSON.parse(localStorage.getItem('taggedPosts') || '{}')[username] || [];
  const taggedPosts = posts.filter(post => taggedPostIds.includes(post.id));

  const handleEditPost = (postId: string, updates: { caption: string; location?: string }) => {
    updatePost(postId, updates);
    toast.success('Post updated successfully');
  };

  const handleDeletePost = (postId: string) => {
    removePost(postId);
    toast.success('Post deleted successfully');
  };

  const handleSaveProfile = (updates: Partial<typeof currentUser>) => {
    updateProfile(updates);
  };

  const handleFollowToggle = () => {
    if (!username) return;
    
    const followData = JSON.parse(localStorage.getItem('followData') || '{}');
    
    if (!followData[currentUsername]) {
      followData[currentUsername] = { following: [], followers: [] };
    }
    if (!followData[username]) {
      followData[username] = { following: [], followers: [] };
    }
    
    const newFollowing = !isFollowing;
    
    if (newFollowing) {
      // Add to current user's following
      if (!followData[currentUsername].following.includes(username)) {
        followData[currentUsername].following.push(username);
      }
      // Add to target user's followers
      if (!followData[username].followers.includes(currentUsername)) {
        followData[username].followers.push(currentUsername);
      }
      toast.success(`Following ${username}`);
    } else {
      // Remove from current user's following
      followData[currentUsername].following = followData[currentUsername].following.filter(
        (u: string) => u !== username
      );
      // Remove from target user's followers
      followData[username].followers = followData[username].followers.filter(
        (u: string) => u !== currentUsername
      );
      toast.success(`Unfollowed ${username}`);
    }
    
    localStorage.setItem('followData', JSON.stringify(followData));
    setIsFollowing(newFollowing);
  };

  if (!user) {
    return (
      <div className="min-h-screen bg-background flex items-center justify-center">
        <p className="text-muted-foreground">User not found</p>
      </div>
    );
  }

  const displayPosts = activeTab === 'posts' ? userPosts : activeTab === 'reposts' ? reposts : taggedPosts;

  const handleShareProfile = () => {
    const profileUrl = window.location.href;
    navigator.clipboard.writeText(profileUrl);
    toast.success('Profile link copied to clipboard!');
  };

  return (
    <div className="min-h-screen bg-background pb-16">
      <Navigation />
      
      <main className="max-w-4xl mx-auto pt-16 pb-4 px-4">
        {/* Profile Header */}
        <section className="mb-12" data-profile-username={user.username}>
          <div className="flex items-start gap-8 mb-8">
            <Avatar className="h-32 w-32">
              <AvatarImage src={user.avatar} alt={user.fullName} />
              <AvatarFallback className="text-3xl">{user.username[0].toUpperCase()}</AvatarFallback>
            </Avatar>

            <div className="flex-1 space-y-4">
              <div className="flex items-center gap-4">
                <h1 className="text-2xl font-normal text-foreground">{user.username}</h1>
                {user.verified && (
                  <span className="text-verified-blue text-sm">✓ Verified</span>
                )}
                {isOwnProfile ? (
                  <>
                    <Button 
                      variant="secondary" 
                      size="sm"
                      onClick={() => setEditProfileOpen(true)}
                    >
                      Edit Profile
                    </Button>
                    <DropdownMenu>
                      <DropdownMenuTrigger asChild>
                        <Button variant="ghost" size="icon">
                          <Menu className="h-5 w-5" />
                        </Button>
                      </DropdownMenuTrigger>
                      <DropdownMenuContent align="end">
                        <DropdownMenuItem onClick={() => window.location.href = '/liked'}>
                          <Bookmark className="mr-2 h-4 w-4" />
                          Liked Posts
                        </DropdownMenuItem>
                        <DropdownMenuItem onClick={handleShareProfile}>
                          <Share2 className="mr-2 h-4 w-4" />
                          Share Profile
                        </DropdownMenuItem>
                      </DropdownMenuContent>
                    </DropdownMenu>
                  </>
                ) : (
                  <>
                    <Button 
                      variant={isFollowing ? "secondary" : "default"} 
                      size="sm"
                      onClick={handleFollowToggle}
                    >
                      {isFollowing ? 'Following' : 'Follow'}
                    </Button>
                    <Button 
                      variant="ghost" 
                      size="icon"
                      onClick={handleShareProfile}
                    >
                      <Share2 className="h-5 w-5" />
                    </Button>
                  </>
                )}
              </div>

              <div className="flex gap-8" data-stats>
                <div>
                  <span className="font-semibold text-foreground">{userPosts.length}</span>{' '}
                  <span className="text-foreground">posts</span>
                </div>
                <button 
                  onClick={() => setFollowersOpen(true)}
                  className="hover:opacity-70 transition-opacity"
                >
                  <span className="font-semibold text-foreground">
                    {(followData[username]?.followers?.length || 0).toLocaleString()}
                  </span>{' '}
                  <span className="text-foreground">followers</span>
                </button>
                <button 
                  onClick={() => setFollowingOpen(true)}
                  className="hover:opacity-70 transition-opacity"
                >
                  <span className="font-semibold text-foreground">
                    {(followData[username]?.following?.length || 0).toLocaleString()}
                  </span>{' '}
                  <span className="text-foreground">following</span>
                </button>
              </div>

              <div>
                <p className="font-semibold text-foreground">{user.fullName}</p>
                <p className="text-foreground whitespace-pre-line">{user.bio}</p>
              </div>
            </div>
          </div>
        </section>

        <Separator className="mb-6" />

        {/* Tabs */}
        <div className="flex justify-center gap-12 mb-8">
          <Button 
            variant="ghost" 
            className={`flex items-center gap-2 pt-4 ${
              activeTab === 'posts' ? 'border-t-2 border-foreground' : 'text-muted-foreground'
            }`}
            onClick={() => setActiveTab('posts')}
          >
            <Grid className="h-4 w-4" />
            <span className="text-xs font-semibold uppercase">Posts</span>
          </Button>
          <Button 
            variant="ghost" 
            className={`flex items-center gap-2 pt-4 ${
              activeTab === 'reposts' ? 'border-t-2 border-foreground' : 'text-muted-foreground'
            }`}
            onClick={() => setActiveTab('reposts')}
          >
            <Repeat2 className="h-4 w-4" />
            <span className="text-xs font-semibold uppercase">Reposts</span>
          </Button>
          <Button 
            variant="ghost" 
            className={`flex items-center gap-2 pt-4 ${
              activeTab === 'tagged' ? 'border-t-2 border-foreground' : 'text-muted-foreground'
            }`}
            onClick={() => setActiveTab('tagged')}
          >
            <Tag className="h-4 w-4" />
            <span className="text-xs font-semibold uppercase">Tagged</span>
          </Button>
        </div>

        {/* Posts Grid */}
        {displayPosts.length === 0 ? (
          <div className="text-center py-20">
            {activeTab === 'posts' && <Grid className="h-16 w-16 mx-auto mb-4 text-muted-foreground" />}
            {activeTab === 'reposts' && <Repeat2 className="h-16 w-16 mx-auto mb-4 text-muted-foreground" />}
            {activeTab === 'tagged' && <Tag className="h-16 w-16 mx-auto mb-4 text-muted-foreground" />}
            <p className="text-muted-foreground text-lg">
              {activeTab === 'posts' && 'No posts yet'}
              {activeTab === 'reposts' && 'No reposts yet'}
              {activeTab === 'tagged' && 'No tagged posts'}
            </p>
          </div>
        ) : (
          <section className="grid grid-cols-3 gap-1" data-section="profile-posts">
            {displayPosts.map((post) => (
              <div 
                key={post.id} 
                className="aspect-square overflow-hidden group relative"
                onMouseEnter={() => setHoveredPostId(post.id)}
                onMouseLeave={() => setHoveredPostId(null)}
              >
                <Link to={`/post/${post.id}`}>
                  <img
                    src={post.image}
                    alt={post.caption}
                    className="w-full h-full object-cover group-hover:scale-105 transition-transform duration-300"
                    loading="lazy"
                  />
                </Link>
                
                {/* Edit/Delete buttons for own posts */}
                {isOwnProfile && activeTab === 'posts' && hoveredPostId === post.id && (
                  <div className="absolute top-2 right-2 flex gap-2 z-10">
                    <Button
                      size="icon"
                      variant="secondary"
                      className="h-8 w-8 bg-background/90 hover:bg-background"
                      onClick={(e) => {
                        e.preventDefault();
                        setEditingPost(post);
                      }}
                    >
                      <Edit2 className="h-4 w-4" />
                    </Button>
                    <Button
                      size="icon"
                      variant="destructive"
                      className="h-8 w-8 bg-destructive/90 hover:bg-destructive"
                      onClick={(e) => {
                        e.preventDefault();
                        setDeletingPost(post);
                      }}
                    >
                      <Trash2 className="h-4 w-4" />
                    </Button>
                  </div>
                )}
              </div>
            ))}
          </section>
        )}

        {/* Dialogs */}
        {username && (
          <>
            <FollowersDialog
              open={followersOpen}
              onOpenChange={setFollowersOpen}
              type="followers"
              username={username}
            />
            <FollowersDialog
              open={followingOpen}
              onOpenChange={setFollowingOpen}
              type="following"
              username={username}
            />
          </>
        )}

        <EditPostDialog
          post={editingPost}
          open={!!editingPost}
          onOpenChange={(open) => !open && setEditingPost(null)}
          onSave={handleEditPost}
        />

        <DeletePostDialog
          post={deletingPost}
          open={!!deletingPost}
          onOpenChange={(open) => !open && setDeletingPost(null)}
          onDelete={handleDeletePost}
        />

        {user && (
          <EditProfileDialog
            open={editProfileOpen}
            onOpenChange={setEditProfileOpen}
            user={user}
            onSave={handleSaveProfile}
          />
        )}

        <CreatePostDialog
          open={createPostOpen}
          onOpenChange={setCreatePostOpen}
          onPostCreated={addPost}
        />

        <CreateStoryDialog
          open={createStoryOpen}
          onOpenChange={setCreateStoryOpen}
          onStoryCreated={() => {}}
        />
      </main>

      <BottomNavigation onCreatePost={() => setCreatePostOpen(true)} />
    </div>
  );
};

export default Profile;
