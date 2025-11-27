import { useState, useMemo } from 'react';
import { Navigation } from '@/components/Navigation';
import { BottomNavigation } from '@/components/BottomNavigation';
import { CreatePostDialog } from '@/components/CreatePostDialog';
import { Input } from '@/components/ui/input';
import { Tabs, TabsContent, TabsList, TabsTrigger } from '@/components/ui/tabs';
import { Avatar, AvatarImage, AvatarFallback } from '@/components/ui/avatar';
import { Search, Heart, MessageCircle } from 'lucide-react';
import { Link } from 'react-router-dom';
import { usePosts } from '@/hooks/useLocalStorage';
import { getUsers } from '@/lib/localStorage';

const Explore = () => {
  const { posts, addPost } = usePosts();
  const users = getUsers();
  const [searchQuery, setSearchQuery] = useState('');
  const [createPostOpen, setCreatePostOpen] = useState(false);

  // Search logic
  const searchResults = useMemo(() => {
    const query = searchQuery.toLowerCase().trim();
    
    if (!query) {
      return {
        posts: posts,
        users: users,
        tags: [],
      };
    }

    // Search posts by caption, username, or location
    const matchedPosts = posts.filter(post => 
      post.caption.toLowerCase().includes(query) ||
      post.username.toLowerCase().includes(query) ||
      post.location?.toLowerCase().includes(query)
    );

    // Search users by username or full name
    const matchedUsers = users.filter(user =>
      user.username.toLowerCase().includes(query) ||
      user.fullName.toLowerCase().includes(query)
    );

    // Extract hashtags from search query
    const hashtags = query.startsWith('#') 
      ? posts.filter(post => post.caption.toLowerCase().includes(query))
      : [];

    return {
      posts: matchedPosts,
      users: matchedUsers,
      tags: hashtags,
    };
  }, [searchQuery, posts, users]);

  return (
    <div className="min-h-screen bg-background pb-16">
      <Navigation />
      
      <main className="max-w-6xl mx-auto pt-16 pb-4 px-4">
        {/* Search Bar */}
        <div className="mb-8">
          <div className="relative max-w-md mx-auto">
            <Search className="absolute left-3 top-1/2 -translate-y-1/2 h-5 w-5 text-muted-foreground" />
            <Input
              type="search"
              placeholder="Search users, posts, or hashtags..."
              className="pl-10 bg-secondary border-none"
              value={searchQuery}
              onChange={(e) => setSearchQuery(e.target.value)}
            />
          </div>
        </div>

        {/* Search Results or Explore Grid */}
        {searchQuery ? (
          <Tabs defaultValue="top" className="w-full">
            <TabsList className="grid w-full max-w-md mx-auto grid-cols-3 mb-8">
              <TabsTrigger value="top">Top</TabsTrigger>
              <TabsTrigger value="accounts">Accounts</TabsTrigger>
              <TabsTrigger value="tags">Tags</TabsTrigger>
            </TabsList>

            <TabsContent value="top" className="space-y-8">
              {/* Users */}
              {searchResults.users.length > 0 && (
                <div>
                  <h3 className="text-sm font-semibold text-muted-foreground mb-4">ACCOUNTS</h3>
                  <div className="space-y-3">
                    {searchResults.users.slice(0, 5).map((user) => (
                      <Link 
                        key={user.id}
                        to={`/profile/${user.username}`}
                        className="flex items-center gap-3 p-2 rounded-lg hover:bg-muted transition-colors"
                      >
                        <Avatar className="h-11 w-11">
                          <AvatarImage src={user.avatar} alt={user.username} />
                          <AvatarFallback>{user.username[0].toUpperCase()}</AvatarFallback>
                        </Avatar>
                        <div className="flex-1">
                          <p className="text-sm font-semibold">{user.username}</p>
                          <p className="text-xs text-muted-foreground">{user.fullName}</p>
                        </div>
                      </Link>
                    ))}
                  </div>
                </div>
              )}

              {/* Posts */}
              {searchResults.posts.length > 0 && (
                <div>
                  <h3 className="text-sm font-semibold text-muted-foreground mb-4">POSTS</h3>
                  <div className="grid grid-cols-3 gap-1">
                    {searchResults.posts.map((post) => (
                      <Link
                        key={post.id}
                        to={`/post/${post.id}`}
                        className="aspect-square overflow-hidden group relative"
                      >
                        <img
                          src={post.image}
                          alt={post.caption}
                          className="w-full h-full object-cover group-hover:scale-105 transition-transform duration-300"
                          loading="lazy"
                        />
                        <div className="absolute inset-0 bg-black/40 opacity-0 group-hover:opacity-100 transition-opacity flex items-center justify-center gap-6 text-white">
                          <div className="flex items-center gap-2">
                            <Heart className="h-5 w-5 fill-white" />
                            <span className="font-semibold">{post.likes}</span>
                          </div>
                          <div className="flex items-center gap-2">
                            <MessageCircle className="h-5 w-5 fill-white" />
                            <span className="font-semibold">{post.comments.length}</span>
                          </div>
                        </div>
                      </Link>
                    ))}
                  </div>
                </div>
              )}

              {searchResults.posts.length === 0 && searchResults.users.length === 0 && (
                <div className="text-center py-12">
                  <p className="text-muted-foreground">No results found for "{searchQuery}"</p>
                  <p className="text-sm text-muted-foreground mt-2">Try searching for something else</p>
                </div>
              )}
            </TabsContent>

            <TabsContent value="accounts">
              {searchResults.users.length > 0 ? (
                <div className="space-y-3 max-w-2xl mx-auto">
                  {searchResults.users.map((user) => (
                    <Link 
                      key={user.id}
                      to={`/profile/${user.username}`}
                      className="flex items-center gap-3 p-3 rounded-lg hover:bg-muted transition-colors"
                    >
                      <Avatar className="h-14 w-14">
                        <AvatarImage src={user.avatar} alt={user.username} />
                        <AvatarFallback>{user.username[0].toUpperCase()}</AvatarFallback>
                      </Avatar>
                      <div className="flex-1">
                        <p className="font-semibold">{user.username}</p>
                        <p className="text-sm text-muted-foreground">{user.fullName}</p>
                        <p className="text-xs text-muted-foreground">
                          {user.followers.toLocaleString()} followers
                        </p>
                      </div>
                    </Link>
                  ))}
                </div>
              ) : (
                <div className="text-center py-12">
                  <p className="text-muted-foreground">No accounts found</p>
                </div>
              )}
            </TabsContent>

            <TabsContent value="tags">
              <div className="text-center py-12">
                <p className="text-muted-foreground">Search for hashtags like #sunset, #food, etc.</p>
              </div>
            </TabsContent>
          </Tabs>
        ) : (
          <section className="grid grid-cols-3 gap-1" data-section="explore-grid">
            {posts.map((post) => (
              <Link
                key={post.id}
                to={`/post/${post.id}`}
                className="aspect-square overflow-hidden group relative"
                data-post-id={post.id}
              >
                <img
                  src={post.image}
                  alt={post.caption}
                  className="w-full h-full object-cover group-hover:scale-105 transition-transform duration-300"
                  loading="lazy"
                />
                <div className="absolute inset-0 bg-black/40 opacity-0 group-hover:opacity-100 transition-opacity flex items-center justify-center gap-6 text-white">
                  <div className="flex items-center gap-2">
                    <Heart className="h-5 w-5 fill-white" />
                    <span className="font-semibold">{post.likes}</span>
                  </div>
                  <div className="flex items-center gap-2">
                    <MessageCircle className="h-5 w-5 fill-white" />
                    <span className="font-semibold">{post.comments.length}</span>
                  </div>
                </div>
              </Link>
            ))}
          </section>
        )}
      </main>

      <BottomNavigation onCreatePost={() => setCreatePostOpen(true)} />
      
      <CreatePostDialog
        open={createPostOpen}
        onOpenChange={setCreatePostOpen}
        onPostCreated={addPost}
      />
    </div>
  );
};

export default Explore;
